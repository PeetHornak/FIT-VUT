<?php
/**
 * Created by PhpStorm.
 * User: Peto
 * Date: 11.3.2019
 */

class Tester
{
    private $tests_dir;
    private $parse_script_dir;
    private $int_script_dir;
    private $recursive;
    private $parse_only;
    private $int_only;
    private $argc;
    private $html;

    public function __construct($argc)
    {
        $this->tests_dir = '.';
        $this->parse_script_dir = 'parse.php';
        $this->int_script_dir = 'interpret.py';
        $this->recursive = false;
        $this->parse_only = false;
        $this->int_only = false;
        $this->argc = $argc;
        $this->html = new DOMDocument('1.0', 'UTF-8');
    }

    public function check_params()
    {
        $params = ['help', 'directory::', 'recursive', 'parse-script::', 'int-script::', 'parse-only', 'int-only'];
        $parsed_params = getopt('', $params);

        if ($this->argc-1 > sizeof($parsed_params)) {
            exit(10);
        }

        foreach ($parsed_params as $param_key => $value){
            if (strpos($param_key, '::')){
                $param_key = substr($param_key, 0, -2);
            }
            if (!key_exists($param_key, $parsed_params)){
                exit(10);
            }
            elseif ($param_key == 'help'){
                $this->print_help();
                exit(0);
            }
            elseif ($param_key == 'directory' and $parsed_params[$param_key]){
                if (substr($parsed_params[$param_key], -1) == '/') {
                    $this->tests_dir = substr_replace($parsed_params[$param_key], "", -1);
                }
                else{
                    $this->tests_dir = $parsed_params[$param_key];
                }
                continue;
            }
            elseif ($param_key == 'recursive'){
                $this->recursive = true;
                continue;
            }
            elseif ($param_key == 'parse-script' and $parsed_params[$param_key]){
                if (key_exists('int-only', $parsed_params)){
                    exit(10);
                }
                $this->parse_script_dir = $parsed_params[$param_key];
                continue;
            }
            elseif ($param_key == 'int-script' and $parsed_params[$param_key]){
                if (key_exists('parse-only', $parsed_params)){
                    exit(10);
                }
                $this->int_script_dir = $parsed_params[$param_key];
                continue;
            }
            elseif ($param_key == 'parse-only') {
                if (key_exists('int-only', $parsed_params)) {
                    exit(10);
                }
                $this->parse_only = true;
                continue;
            }
            elseif ($param_key == 'int-only') {
                $this->int_only = true;
            }
        }
        $this->check_files();
        echo($this->html->saveHTML());
    }

    private function check_files()
    {
        if (!is_dir($this->tests_dir)){
            exit(11);
        }
        if (!$this->int_only) {
            if (!file_exists($this->parse_script_dir) || !is_writeable($this->parse_script_dir)) {
                exit(11);
            }
        }
        if (!$this->parse_only) {
            if (!file_exists($this->int_script_dir) || !is_writeable($this->int_script_dir)) {
                exit(11);
            }
        }
        $this->find_tests($this->tests_dir);
    }

    public function find_tests($dir)
    {
        $test_files = glob("{$dir}/*.src", GLOB_BRACE);
        foreach ($test_files as $file){
            $file = substr_replace($file , '', strrpos($file , '.'));
            if (!file_exists("{$file}.rc")){
                file_put_contents("{$file}.rc", "0");
            }
            elseif (!is_readable("{$file}.rc")){
                exit(12);
            }
            if (!file_exists("{$file}.out")){
                touch("{$file}.out");
            }
            elseif (!is_readable("{$file}.out")){
                exit(12);
            }
            if (!file_exists("{$file}.in")){
                touch("{$file}.in");
            }
            elseif (!is_readable("{$file}.in")){
                exit(12);
            }
        }
        $this->run_tests($dir, $test_files);
        if ($this->recursive) {
            $directories = $this->find_all_subdirs($dir);
            foreach ($directories as $directory){
                $this->find_tests($directory);
            }
        }
    }

    private function run_tests($dir, $test_files)
    {
        $temp_file = tempnam($dir, 'out');
        foreach ($test_files as $src_file) {
            $rc_file = substr_replace($src_file, 'rc', strrpos($src_file, '.') + 1);
            $out_file = substr_replace($src_file, 'out', strrpos($src_file, '.') + 1);
            $in_file = substr_replace($src_file, 'in', strrpos($src_file, '.') + 1);
            if ($this->parse_only) {
                exec("php7.3 {$this->parse_script_dir} < {$src_file} > {$temp_file}", $out, $retval);
            }
            elseif ($this->int_only) {
                exec("python3.6 {$this->int_script_dir} --source={$src_file} < {$in_file} > {$temp_file}", $out, $retval);
            }
            else{
                exec("php7.3 {$this->parse_script_dir} < {$src_file} > {$temp_file}", $out, $retval);
                if ($retval){
                    $this->test_failed($src_file);
                    continue;
                }
                exec("python3.6 {$this->int_script_dir} --source={$temp_file} < {$in_file} > {$temp_file}", $out, $retval);
            }
            $rc = file_get_contents($rc_file);
            if ($rc != $retval) {
                $this->test_failed_return($src_file, $rc, $retval);
            }
            else {
                if ($this->parse_only) {
                    exec("java -jar /pub/courses/ipp/jexamxml/jexamxml.jar {$temp_file} {$out_file}", $out, $retval);
                }
                else {
                    exec("diff {$temp_file} {$out_file}", $out, $retval);
                }
                if ($retval) {
                    $this->test_failed_output($src_file);
                }
                else {
                    $this->test_passed($src_file);
                }
            }

        }
        unlink($temp_file);
    }

    private function find_all_subdirs($dir)
    {
        $directories = glob("{$dir}/*" , GLOB_ONLYDIR);
        return $directories;
    }

    private function test_failed_output($file){
        $message = "Test {$file} failed: different output";
        $p = $this->html->createElement('p', $message);
        $color_attribute = $this->html->createAttribute('style');
        $color_attribute->value = 'color:red';
        $p->appendChild($color_attribute);
        $this->html->appendChild($p);
    }

    private function test_failed_return($file, $expected_value, $return_value){
        $message = "Test {$file} failed: returned {$return_value} expected {$expected_value}";
        $p = $this->html->createElement('p', $message);
        $color_attribute = $this->html->createAttribute('style');
        $color_attribute->value = 'color:red';
        $p->appendChild($color_attribute);
        $this->html->appendChild($p);
    }

    private function test_passed($file){
        $message = "Test {$file} passed";
        $p = $this->html->createElement('p', $message);
        $color_attribute = $this->html->createAttribute('style');
        $color_attribute->value = 'color:darkgreen';
        $p->appendChild($color_attribute);
        $this->html->appendChild($p);
    }

    private function print_help()
    {
        echo "Usage: php7.3 test.php > STDOUT\n"
            ."Use optional parameters:\n"
            ."\t[--help] - to print this message\n"
            ."\t[--direcory=path] - to set directory of test suite\n"
            ."\t\tdefault: current directory\n"
            ."\t[--parse-script=file] - to set path to parser\n"
            ."\t\tdefault: ./parse.php\n"
            ."\t[--int-script=file] - to set path to interpreter\n"
            ."\t\tdefault: ./interpreter.py\n"
            ."\t[--parse-only] - to test parser.php only\n"
            ."\t[--int-only] - to test interpreter.py only\n"
            ."########################\n"
            ."##Author: Peter Horňák##\n"
            ."##Login: xhorna14     ##\n"
            ."##Date: 13.3.2019     ##\n"
            ."########################\n";
        exit(0);
    }
}

$tester = new Tester($argc);
$tester->check_params();
