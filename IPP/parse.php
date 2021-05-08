<?php
/**
 * Created by PhpStorm.
 * User: Peťo
 * Date: 19.02.2019
 */

class Parser
{
    private $argv;
    private $argc;
    private $xml;
    private $order;
    private $count;
    private $param_order;
    private $comments_count;
    private $labels_count;
    private $jumps_count;
    private $path_to_file;

    public function __construct($argc, $argv)
    {
        $this->argv = $argv;
        $this->argc = $argc;
        $temp = <<<XML
<?xml version="1.0" encoding="utf-8" ?>
    <program language="IPPcode19">
    </program>
XML;
        $this->xml = new SimpleXMLElement($temp);
        $this->order = 1;
        $this->count = 0;
        $this->param_order = [];
        $this->comments_count = 0;
        $this->labels_count=0;
        $this->jumps_count=0;
        $this->path_to_file = '';
    }

    public function parse()
    {
        $this->check_params();
        $line = explode('#', fgets(STDIN));
        if (!preg_match('/.(?i)(ippcode19)/' ,trim($line[0]))) {
            exit(21);
        }
        if (count($line) > 1) {
            $this->comments_count++;
        }
        while ($line = fgets(STDIN)) {
            /*Odelenie komentarov*/
            if (count($matches = explode('#', $line)) != 1) {
                $line = $matches[0];
                $this->comments_count++;
            }
            $pattern = '/(\s)+/';
            $line = trim($line);
            $matches = preg_split($pattern, $line);
            $this->parse_first($matches);
        }
        $this->print_stats();
        echo $this->xml->asXML();
    }

    private function parse_first($matches)
    {
        $this->count = 0;
        $match = $matches[$this->count];
        if($match != ''){
            $instruction = $this->xml->addChild("instruction");
            $instruction->addAttribute("order", $this->order);
            $instruction->addAttribute("opcode", strtoupper($match));
            if (preg_match('/(?i)(READ)/', $match)) {
                $this->count_of_params($matches, 2);
                $this->check_var($matches);
                $this->check_type($matches);
            }
            elseif (preg_match('/^(?i)(PUSHS|WRITE|EXIT|DPRINT)$/', $match)) {
                $this->count_of_params($matches, 1);
                $this->check_symb($matches);
            }
            elseif (preg_match('/^(?i)(CALL|LABEL|JUMP)$/', $match)) {
                $this->count_of_params($matches, 1);
                $this->check_label($matches);
            }
            elseif (preg_match('/^(?i)(JUMPIFEQ|JUMPIFNEQ)$/', $match)) {
                $this->count_of_params($matches, 3);
                $this->check_label($matches);
                $this->check_symb($matches);
                $this->check_symb($matches);
            }
            elseif (preg_match('/^(?i)(ADD|SUB|MUL|IDIV|LT|GT|EQ|AND|OR|STRI2INT|CONCAT|GETCHAR|SETCHAR)$/', $match)) {
                $this->count_of_params($matches, 3);
                $this->check_var($matches);
                $this->check_symb($matches);
                $this->check_symb($matches);
            }
            elseif (preg_match('/^(?i)(NOT|INT2CHAR|STRLEN|TYPE|MOVE)$/', $match)) {
                $this->count_of_params($matches, 2);
                $this->check_var($matches);
                $this->check_symb($matches);
            }
            elseif (preg_match('/^(?i)(DEFVAR|POPS)$/', $match)) {
                $this->count_of_params($matches, 1);
                $this->check_var($matches);
            }
            elseif (preg_match('/^(?i)(CREATEFRAME|PUSHFRAME|POPFRAME|RETURN|BREAK)$/', $match)) {
                $this->count_of_params($matches, 0);
            }
            else {
                exit(22);
            }
            $this->order++;
        }
    }

    private function check_var($matches)
    {
        $this->count++;
        $match = $matches[$this->count];
        if (preg_match('/^(GF|LF|TF)@[\_\-\$\%\*\?\!\&a-zA-Z0-9]+$/', $match)) {
            $arg = $this->xml->instruction[$this->order-1]->addChild("arg$this->count", htmlspecialchars($match));
            $arg->addAttribute('type', 'var');
        }
        else {

            exit(23);
        }
    }

    private function check_symb($matches)
    {
        $this->count++;
        $match = $matches[$this->count];
        if (preg_match('/^(GF|LF|TF)@[\_\-\$\%\*\?\!\&a-zA-Z0-9]+$/', $match)) {
            $arg = $this->xml->instruction[$this->order - 1]->addChild("arg$this->count", htmlspecialchars($match));
            $arg->addAttribute('type', 'var');
        }
        elseif (preg_match('/^(string@(([^\\\\\s])*([\\\\]([0-9]{3}))*([^\\\\\s])*)*)$|^(int@-?[0-9]*)$|^(bool@(true|false)$|^(nil@nil))$/', $match)) {
            $arg = $this->xml->instruction[$this->order - 1]->addChild("arg$this->count", htmlspecialchars(explode('@', $match, 2)[1]));
            $arg->addAttribute('type', explode('@', $match)[0]);
        }
        else {
            exit(23);
        }
    }

    private function check_label($matches)
    {
        if (preg_match('/(?i)LABEL/', $matches[$this->count])) {
            $this->labels_count++;
        }
        elseif (preg_match('/(?i)JUMP/', $matches[$this->count])) {
            $this->jumps_count++;
        }
        $this->count++;
        $match = $matches[$this->count];
        if (preg_match('/^[\_\-\$\%\*\?\!\&a-zA-Z0-9]+$/', $match)) {
            $arg = $this->xml->instruction[$this->order-1]->addChild("arg$this->count", $match);
            $arg->addAttribute('type', 'label');
        }
        else {
            exit(23);
        }
    }

    private function check_type($matches)
    {
        $this->count++;
        $match = $matches[$this->count];
        if (preg_match('/^(int|string|bool)$/', $match)) {
            $arg = $this->xml->instruction[$this->order-1]->addChild("arg$this->count", $match);
            $arg->addAttribute('type', 'type');
        }
        else {
            exit(23);
        }
    }

    private function count_of_params($matches, $params_count)
    {
        if(count($matches)-1 > $params_count) {
            exit(23);
        }
    }

    private function check_params()
    {

        $param_check = ['--loc', '--comments', '--labels', '--jumps'];
        if ($this->argc != 1) {
            if ($this->argc == 2 && $this->argv[1] == '--help') {
                $this->print_help();
            }
            elseif ($this->argc > 2) {
                if ($param = preg_split('/=/', $this->argv[1], 2)) {
                    if($param[0] == '--stats' && count($param) == 2){
                        $this->path_to_file =  $param[1];
                        if (!file_exists($param[1]) || is_writeable($this->path_to_file)) {
                            file_put_contents($param[1], "");
                                foreach (array_slice($this->argv, 2) as $param) {
                                if (in_array($param, $param_check)) {
                                    array_push($this->param_order, $param);
                                }
                                else
                                    exit(10);
                            }
                        }
                        else
                            exit(12);
                    }
                    else
                        exit(10);
                }
                else
                    exit(10);
            }
            else
                exit(10);
        }
    }

    private function print_stats()
    {
        foreach ($this->param_order as $param) {
            if ($param == '--loc') {
                file_put_contents($this->path_to_file, $this->order.PHP_EOL, FILE_APPEND);
            }
            if ($param == '--comments') {
                file_put_contents($this->path_to_file, $this->comments_count.PHP_EOL, FILE_APPEND);
            }
            if ($param == '--labels') {
                file_put_contents($this->path_to_file, $this->labels_count.PHP_EOL, FILE_APPEND);
            }
            if ($param == '--jumps') {
                file_put_contents($this->path_to_file, $this->jumps_count.PHP_EOL, FILE_APPEND);
            }
        }
    }

    private function print_help()
    {
        echo "Usage: php7.3 parse.php < STDIN > STDOUT\n"
            ."Optional parameters:\n"
            ."\t[--help] - to print this message\n"
            ."\t[--stats=file] - to print statistics of source code\n"
            ."\tWith at least one of the following parameters\n"
            ."\t\t[--loc] - to print number of instructions\n"
            ."\t\t[--comments] - to print number of comments\n"
            ."\t\t[--labels] - to print number of defined labels\n"
            ."\t\t[--jumps] - to print number of jump instructions\n\n"
            ."########################\n"
            ."##Author: Peter Horňák##\n"
            ."##Login: xhorna14     ##\n"
            ."##Date: 19.2.2019     ##\n"
            ."########################\n";
        exit(0);
    }
}


$parser = new Parser($argc, $argv);
$parser->parse();
