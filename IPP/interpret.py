import xml.etree.ElementTree as ET
import argparse
from sys import stdin, stderr
import os
import re

symbs = 'string', 'int', 'bool', 'nil', 'var'
types = 'string', 'int', 'bool', 'var'


class Frame:
    def __init__(self):
        self.locals = {}

    def def_var(self, variable):
        if variable not in self.locals:
            self.locals[variable] = None
        else:
            stderr.write('DEFVAR: Redefinition of variable\n')
            exit(52)

    def set_value(self, variable, value):
        my_type = value[0]
        my_value = value[1]
        if variable in self.locals:
            self.locals[variable] = (my_type, my_value)
        else:
            stderr.write('Working with not defined variable\n')
            exit(54)

    def get_value(self, variable):
        if variable in self.locals:
            if self.locals[variable] is None:
                stderr.write('Trying to get value of not declared variable\n')
                exit(56)
            return self.locals[variable]
        else:
            stderr.write('Working with not defined variable\n')
            exit(54)


class Label(Frame):
    def __init__(self):
        super().__init__()

    def def_var(self, variable):
        name, order = variable
        if name not in self.locals:
            self.locals[name] = order
        else:
            stderr.write('Redefinition of label\n')
            exit(52)

    def get_value(self, label):
        if label in self.locals:
            return self.locals[label]
        else:
            stderr.write('Non existing label\n')
            exit(52)


# noinspection PyUnboundLocalVariable
class Interpreter:
    def __init__(self, source, input_f):
        self.source = source
        self.input_file = input_f
        self.input = None
        self.frame_stack = []
        self.data_stack = []
        self.call_stack = []
        self.to_execute = {}
        self.frames = {'GF': Frame(),
                       'LF': None,
                       'TF': None}
        self.labels = Label()
        self.current_order = 1

    def parse_src(self):
        # TODO: READ SRC FROM STDIN!!!
        try:
            tree = ET.parse(self.source)
        except ET.ParseError:
            stderr.write('Wrong xml input\n')
            exit(31)
        root = tree.getroot()
        language = root.get('language')
        if not language == 'IPPcode19':
            stderr.write('Wrong header language\n')
            exit(32)

        for instruction in root.findall('instruction'):
            self.parse_instruction(instruction)

        if self.input_file is not stdin:
                with open(self.input_file, 'r') as input_file:
                    self.input = input_file.read().splitlines()

    def parse_instruction(self, instruction):
        if 'order' in instruction.attrib and 'opcode' in instruction.attrib \
                                        and len(instruction.attrib) == 2:
            args = {'arg1': None, 'arg2': None, 'arg3': None}
            for arg in instruction:
                if not (len(arg.keys()) == 1 and 'type' in arg.keys()):
                    stderr.write('Wrong arg attributes\n')
                    exit(32)
                try:
                    if args[arg.tag] is None:
                        args[arg.tag] = (arg.get('type'), arg.text)
                    else:
                        stderr.write('Redefinition of argument\n')
                        exit(32)
                except:
                    stderr.write('Wrong instruction sub element\n')
                    exit(32)
            if args['arg3'] is not None:
                if args['arg2'] is not None:
                    if args['arg1'] is None:
                        stderr.write('Wrong argument index\n')
                        exit(32)
                else:
                    stderr.write('Wrong argument index\n')
                    exit(32)
            if args['arg2'] is not None and args['arg1'] is None:
                stderr.write('Wrong argument index\n')
                exit(32)

            key = instruction.attrib['order']
            to_execute = (instruction.attrib['opcode'], args)
            if key in self.to_execute:
                stderr.write('Multiple instructions with same order number\n')
                exit(32)
            self.to_execute[key] = to_execute
        else:
            stderr.write('Wrong instruction attributes\n')
            exit(31)

    def check_execute_order(self):
        for key in self.to_execute.keys():
            if int(key) not in range(1, len(self.to_execute)+1):
                stderr.write('Wrong instruction order\n')
                exit(32)


    def execute(self):
        self.check_execute_order()
        for i in range(2):
            self.current_order = 1
            while self.current_order <= len(self.to_execute):
                opcode = self.to_execute[str(self.current_order)][0]
                if i == 0:
                    if not opcode == 'LABEL':
                        self.current_order += 1
                        continue
                else:
                    if opcode == 'LABEL':
                        self.current_order += 1
                        continue
                args = [value for key, value in self.to_execute[str(self.current_order)][1].items() if value is not None]
                try:
                    func = getattr(self, opcode)
                except AttributeError:
                    stderr.write('Wrong opcode\n')
                    exit(32)
                if not args:
                    try:
                        func()
                    except TypeError:
                        stderr.write('Wrong number of arguments passed to instruction\n')
                        exit(52)
                else:
                    try:
                        func(*args)
                    except TypeError:
                        stderr.write('Wrong number of arguments passed to instruction\n')
                        exit(52)
                self.current_order += 1
            #
            # print(opcode)
            # for frame, values in self.frames.items():
            #     if values is None:
            #         print("{}: None".format(frame))
            #         continue
            #     print("{}: {}".format(frame, values.locals))
            # print("===============")

    @staticmethod
    def top(stack):
        return stack[-1]

    @staticmethod
    def pop(stack):
        if stack:
            return stack.pop()
        else:
            return None

    @staticmethod
    def push(stack, value):
        stack.append(value)

    @staticmethod
    def check_operand(expected_type, var, err=True):
        var_type = var[0]
        ret = None
        if var_type not in expected_type:
            stderr.write('Wrong type of operand\n')
            exit(53)
        variable = var[1]
        if var_type == 'var':
            ret = re.match(r'(GF|LF|TF)@([_\-$&%*!?a-zA-Z]+[_\-$&%*!?a-zA-Z0-9]*)', variable)
            if ret is None:
                stderr.write('Wrong format of <var>\n')
                exit(32)
            frame = ret.group(1)
            value = ret.group(2)
            return frame, value
        else:
            if var_type == 'string':
                ret = re.match(r'(\S)*', variable)  # TODO: escape sequences
            elif var_type == 'int':
                ret = re.match(r'-?[0-9]+', variable)
            elif var_type == 'bool':
                ret = re.match(r'true$|false$', variable, re.IGNORECASE)
            elif var_type == 'nil':
                ret = re.match(r'nil$', variable)
            elif var_type == 'label':
                ret = re.match(r'(\S)+', variable)
            else:
                stderr.write('Wrong type of operand\n')
                exit(53)

            if ret is None:
                if err:
                    stderr.write('Wrong format of variable\n')
                    exit(32)
                else:
                    return var_type, ""
            else:
                value = ret.string
                return var_type, value

    @staticmethod
    def check_bool(value):
        if re.match(r'true$', value, re.IGNORECASE):
            return True
        elif re.match(r'false$', value, re.IGNORECASE):
            return False
        else:
            return value

    def arithmetic_instruct(self, var, symb1, symb2):
        var_frame, var_value = self.check_operand('var', var)
        first_frame, first_value = self.check_operand(('var', 'int'), symb1)
        first_type, first_value = self.check_variable(first_frame, first_value, 'int')
        second_frame, second_value = self.check_operand(('var', 'int'), symb2)
        second_type, second_value = self.check_variable(second_frame, second_value, 'int')
        # if not first_frame == 'int':
        #     if self.frames[first_frame] is None:
        #         stderr.write('ARITHMETIC: Working with not existing frame\n')
        #         exit(55)
        #     first_type, first_value = self.frames[first_frame].get_value(first_value)
        # if not second_frame == 'int':
        #     if self.frames[second_frame] is None:
        #         stderr.write('ARITHMETIC: Working with not existing frame\n')
        #         exit(55)
        #     first_type, second_value = self.frames[second_frame].get_value(second_value)
        # if not first_type == 'int' or not second_type == 'int':
        #     stderr.write('ARITHMETIC: Wrong type of operand\n')
        #     print(first_type, second_type)
        #     exit(53)
        return first_value, second_value, var_frame, var_value

    def relation_operators(self, var, symb1, symb2, label):
        if label:
            var_frame, var_value = self.check_operand('label', var)
        else:
            var_frame, var_value = self.check_operand('var', var)
        first_frame, first_value = self.check_operand(types, symb1)
        second_frame, second_value = self.check_operand(types, symb2)
        first_type = first_frame
        second_type = second_frame
        if first_frame not in types:
            if self.frames[first_frame] is None:
                stderr.write('LT: Working with not existing frame\n')
                exit(55)
            first_type, first_value = self.frames[first_frame].get_value(first_value)
        if second_frame not in types:
            if self.frames[second_frame] is None:
                stderr.write('LT: Working with not existing frame\n')
                exit(55)
            second_type, second_value = self.frames[second_frame].get_value(second_value)
        if not first_type == second_type:
            stderr.write('LT: Different types of operands\n')
            exit(53)
        if first_type == 'bool':
            first_value = self.check_bool(first_value)
        if second_type == 'bool':
            second_value = self.check_bool(second_value)

        return first_value, second_value, var_frame, var_value

    def boolean_operators(self, var, symb1, symb2):
        var_frame, var_value = self.check_operand('var', var)
        first_frame, first_value = self.check_operand(('bool', 'var'), symb1)
        second_frame, second_value = self.check_operand(('bool', 'var'), symb2)
        first_type = first_frame
        second_type = second_frame
        if not first_frame == 'bool':
            if self.frames[first_frame] is None:
                stderr.write('AND: Working with not existing frame\n')
                exit(55)
            first_type, first_value = self.frames[first_frame].get_value(first_value)
        if not second_frame == 'bool':
            if self.frames[second_frame] is None:
                stderr.write('AND: Working with not existing frame\n')
                exit(55)
            second_type, second_value = self.frames[second_frame].get_value(second_value)
        if not first_type == 'bool' or not second_type == 'bool':
            stderr.write('AND: Operands are not boolean\n')
            exit(53)
        first_value = self.check_bool(first_value)
        second_value = self.check_bool(second_value)
        return first_value, second_value, var_frame, var_value

    def check_variable(self, frame, value, type_to_check):
        my_type = type_to_check
        if frame not in type_to_check:      # TODO: maybe fix to: not frame == type_to_check
            if self.frames[frame] is None:
                stderr.write('Working with not existing frame\n')
                exit(55)
            my_type, value = self.frames[frame].get_value(value)
            if not my_type == type_to_check:
                stderr.write('Wrong operand types\n')
                exit(53)
        return my_type, value

    def MOVE(self, var, symb):
        var_frame, var_value = self.check_operand('var', var)
        assign_frame,  assign_value = self.check_operand(symbs, symb)
        if self.frames[var_frame] is None:
            stderr.write('MOVE: Working with not existing frame\n')
            exit(55)

        if assign_frame in symbs:
            self.frames[var_frame].set_value(var_value, (assign_frame, assign_value))
        else:
            value_to_set = self.frames[assign_frame].get_value(assign_value)
            self.frames[var_frame].set_value(var_value, value_to_set)

    def CREATEFRAME(self):
        self.frames['TF'] = Frame()

    def PUSHFRAME(self):
        if self.frames['TF'] is not None:
            self.push(self.frame_stack, self.frames['TF'])
            self.frames['LF'] = self.top(self.frame_stack)
            self.frames['TF'] = None
        else:
            stderr.write('PUSHFRAME: Pushing not defined frame\n')
            exit(55)

    def POPFRAME(self):
        if self.frame_stack:
            self.frames['TF'] = self.pop(self.frame_stack)
            if self.frame_stack:
                self.frames['LF'] = self.top(self.frame_stack)
            else:
                self.frames['LF'] = None
        else:
            stderr.write('POPFRAME: Pop from empty stack\n')
            exit(55)

    def DEFVAR(self, var):
        frame, value = self.check_operand('var', var)
        if self.frames[frame] is None:
            stderr.write('DEFVAR: Working with not existing frame\n')
            exit(55)
        self.frames[frame].def_var(value)

    def CALL(self, label):
        self.push(self.call_stack, self.current_order)
        order = self.labels.get_value(label[1])
        self.current_order = order

    def RETURN(self):
        self.current_order = self.pop(self.call_stack)
        if self.current_order is None:
            stderr.write('RETURN: Pop from empty call stack')
            exit(56)

    def PUSHS(self, symb):
        frame, var_value = self.check_operand(symbs, symb)
        if frame in symbs:
            self.push(self.data_stack, (frame, var_value))
        else:
            if self.frames[frame] is None:
                stderr.write('PUSHS: Working with not existing frame\n')
                exit(55)
            to_push_value = self.frames[frame].get_value(var_value)
            self.push(self.data_stack, to_push_value)

    def POPS(self, var):
        frame, var_value = self.check_operand('var', var)
        to_assign_value = self.pop(self.data_stack)
        if self.frames[frame] is None:
            stderr.write('POPS: Working with not existing frame\n')
            exit(55)
        self.frames[frame].set_value(var_value, to_assign_value)

    def ADD(self, var, symb1, symb2):
        first_value, second_value, var_frame, var_value = self.arithmetic_instruct(var, symb1, symb2)
        final_value = int(first_value) + int(second_value)
        if self.frames[var_frame] is None:
            stderr.write('ADD: Working with not existing frame\n')
            exit(55)
        self.frames[var_frame].set_value(var_value, ('int', final_value))

    def SUB(self, var, symb1, symb2):
        first_value, second_value, var_frame, var_value = self.arithmetic_instruct(var, symb1, symb2)
        final_value = int(first_value) - int(second_value)
        if self.frames[var_frame] is None:
            stderr.write('SUB: Working with not existing frame\n')
            exit(55)
        self.frames[var_frame].set_value(var_value, ('int', final_value))

    def MUL(self, var, symb1, symb2):
        first_value, second_value, var_frame, var_value = self.arithmetic_instruct(var, symb1, symb2)
        final_value = int(first_value) * int(second_value)
        if self.frames[var_frame] is None:
            stderr.write('MUL: Working with not existing frame\n')
            exit(55)
        self.frames[var_frame].set_value(var_value, ('int', final_value))

    def IDIV(self, var, symb1, symb2):
        first_value, second_value, var_frame, var_value = self.arithmetic_instruct(var, symb1, symb2)
        if int(second_value) == 0:
            stderr.write('IDIV: Division by 0\n')
            exit(57)
        final_value = int(first_value) // int(second_value)
        if self.frames[var_frame] is None:
            stderr.write('IDIV: Working with not existing frame\n')
            exit(55)
        self.frames[var_frame].set_value(var_value, ('int', final_value))

    def LT(self, var, symb1, symb2):
        first_value, second_value, var_frame, var_value = self.relation_operators(var, symb1, symb2, False)
        final_value = first_value < second_value
        if self.frames[var_frame] is None:
            stderr.write('LT: Working with not existing frame\n')
            exit(55)
        self.frames[var_frame].set_value(var_value, ('bool', final_value))

    def GT(self, var, symb1, symb2):
        first_value, second_value, var_frame, var_value = self.relation_operators(var, symb1, symb2, False)
        final_value = first_value > second_value
        if self.frames[var_frame] is None:
            stderr.write('GT: Working with not existing frame\n')
            exit(55)
        self.frames[var_frame].set_value(var_value, ('bool', final_value))

    def EQ(self, var, symb1, symb2):
        first_value, second_value, var_frame, var_value = self.relation_operators(var, symb1, symb2, False)
        final_value = first_value == second_value
        if self.frames[var_frame] is None:
            stderr.write('EQ: Working with not existing frame\n')
            exit(55)
        self.frames[var_frame].set_value(var_value, ('bool', final_value))

    def AND(self, var, symb1, symb2):
        first_value, second_value, var_frame, var_value = self.boolean_operators(var, symb1, symb2)
        final_value = first_value and second_value
        if self.frames[var_frame] is None:
            stderr.write('AND: Working with not existing frame\n')
            exit(55)
        self.frames[var_frame].set_value(var_value, ('bool', final_value))

    def OR(self, var, symb1, symb2):
        first_value, second_value, var_frame, var_value = self.boolean_operators(var, symb1, symb2)
        final_value = first_value or second_value
        if self.frames[var_frame] is None:
            stderr.write('OR: Working with not existing frame\n')
            exit(55)
        self.frames[var_frame].set_value(var_value, ('bool', final_value))

    def NOT(self, var, symb):
        first_value, second_value, var_frame, var_value = self.boolean_operators(var, symb, symb)
        final_value = not first_value
        if self.frames[var_frame] is None:
            stderr.write('NOT: Working with not existing frame\n')
            exit(55)
        self.frames[var_frame].set_value(var_value, ('bool', final_value))

    def INT2CHAR(self, var, symb):
        var_frame, var_value = self.check_operand('var', var)
        first_frame, first_value = self.check_operand(('int', 'var'), symb)
        first_type, first_value = self.check_variable(first_frame, first_value, 'int')
        try:
            first_value = int(first_value)
            final_value = chr(first_value)
        except OverflowError:
            stderr.write('INT2CHAR: Not valid ordinal value of Unicode\n')
            exit(58)
        except ValueError:
            stderr.write('INT2CHAR: Not valid ordinal value of Unicode\n')
            exit(58)
        if self.frames[var_frame] is None:
            stderr.write('INT2CHAR: Working with not existing frame\n')
            exit(55)
        self.frames[var_frame].set_value(var_value, ('string', final_value))

    def STRI2INT(self, var, symb1, symb2):
        var_frame, var_value = self.check_operand('var', var)
        first_frame, first_value = self.check_operand(('string', 'var'), symb1)
        first_type, first_value = self.check_variable(first_frame, first_value, 'string')
        second_frame, second_value = self.check_operand(('int', 'var'), symb2)
        second_type, second_value = self.check_variable(second_frame, second_value, 'int')
        if self.frames[var_frame] is None:
            stderr.write('STRI2INT: Working with not existing frame\n')
            exit(55)
        second_value = int(second_value)
        if second_value < 0 or second_value > (len(first_value) - 1):
            stderr.write('STRI2INT: Index out of range\n')
            exit(58)

        final_value = ord(first_value[second_value])
        self.frames[var_frame].set_value(var_value, ('int', final_value))

    def READ(self, var, my_type):
        var_frame, var_value = self.check_operand('var', var)
        type_type, type_value = my_type
        if not type_type == 'type':
            stderr.write('READ: Wrong type of operand\n')
            exit(53)
        if not self.input_file == stdin:
            my_input = self.input[0]
            self.input = self.input[1:]
        else:
            try:
                my_input = input()
            except EOFError:
                my_input = ""

        type_type, type_value = self.check_operand(type_value, (type_value, my_input), False)
        if type_value == "":
            if type_type == 'int':
                type_value = 0
            if type_type == 'bool':
                type_value = 'false'
        if self.frames[var_frame] is None:
            stderr.write('READ: Working with not existing frame\n')
            exit(55)

        self.check_bool(type_value)
        self.frames[var_frame].set_value(var_value, (type_type, type_value))

    def WRITE(self, symb):
        frame, value = self.check_operand(symbs, symb)
        if frame not in symbs:
            if self.frames[frame] is None:
                stderr.write('WRITE: Working with not existing frame\n')
                exit(55)
            frame, value = self.frames[frame].get_value(value)
        if frame == 'nil':
            print('', end='')
        if frame == 'bool':
            if value:
                print('true', end='')
            else:
                print('false', end='')
        else:
            print(value, end='')

    def CONCAT(self, var, symb1, symb2):
        var_frame, var_value = self.check_operand('var', var)
        first_frame, first_value = self.check_operand(('string', 'var'), symb1)
        first_type, first_value = self.check_variable(first_frame, first_value, 'string')
        second_frame, second_value = self.check_operand(('string', 'var'), symb2)
        second_type, second_value = self.check_variable(second_frame, second_value, 'string')

        final_value = first_value + second_value
        if self.frames[var_frame] is None:
            stderr.write('CONCAT: Working with not existing frame\n')
            exit(55)
        self.frames[var_frame].set_value(var_value, ('string', final_value))

    def STRLEN(self, var, symb):
        var_frame, var_value = self.check_operand('var', var)
        first_frame, first_value = self.check_operand(('string', 'var'), symb)
        first_type, first_value = self.check_variable(first_frame, first_value, 'string')

        final_value = len(first_value)
        if self.frames[var_frame] is None:
            stderr.write('STRLEN: Working with not existing frame\n')
            exit(55)
        self.frames[var_frame].set_value(var_value, ('int', final_value))

    def GETCHAR(self, var, symb1, symb2):
        var_frame, var_value = self.check_operand('var', var)
        first_frame, first_value = self.check_operand(('string', 'var'), symb1)
        first_type, first_value = self.check_variable(first_frame, first_value, 'string')
        second_frame, second_value = self.check_operand(('int', 'var'), symb2)
        second_type, second_value = self.check_variable(second_frame, second_value, 'int')

        second_value = int(second_value)
        if second_value < 0 or second_value > (len(first_value) - 1):
            stderr.write('GETCHAR: Index out of range\n')
            exit(58)

        final_value = first_value[second_value]
        if self.frames[var_frame] is None:
            stderr.write('STRLEN: Working with not existing frame\n')
            exit(55)
        self.frames[var_frame].set_value(var_value, ('string', final_value))

    def SETCHAR(self, var, symb1, symb2):
        var_frame, var_value = self.check_operand('var', var)
        first_frame, first_value = self.check_operand(('int', 'var'), symb1)
        first_type, first_value = self.check_variable(first_frame, first_value, 'int')
        second_frame, second_value = self.check_operand(('string', 'var'), symb2)
        second_type, second_value = self.check_variable(second_frame, second_value, 'string')

        first_value = int(first_value)
        if self.frames[var_frame] is None:
            stderr.write('SETCHAR: Working with not existing frame\n')
            exit(55)
        final_type, final_value = self.frames[var_frame].get_value(var_value)
        if not final_type == 'string':
            stderr.write('SETCHAR: Wrong operand type\n')
            exit(53)
        if first_value < 0 or first_value > (len(final_value) - 1):
            stderr.write('SETCHAR: Index out of range\n')
            exit(58)
        if second_value == "":
            stderr.write('SETCHAR: Empty string\n')
            exit(58)
        final_value = final_value[:first_value] + second_value[0] + final_value[first_value + 1:]
        self.frames[var_frame].set_value(var_value, ('string', final_value))

    def TYPE(self, var, symb):
        var_frame, var_value = self.check_operand('var', var)
        first_frame, first_value = self.check_operand(symbs, symb, False)

        if self.frames[var_frame] is None:
            stderr.write('TYPE: Working with not existing frame\n')
            exit(55)
        if first_frame in symbs:
            self.frames[var_frame].set_value(var_value, first_frame)
        else:
            if self.frames[first_frame] is None:
                stderr.write('TYPE: Working with not existing frame\n')
                exit(55)
            final_frame, final_value = self.frames[first_frame].get_value(first_value)
            self.frames[var_frame].set_value(var_value, ('string', final_frame))

    def LABEL(self, label):
        label, value = self.check_operand('label', label)
        self.labels.def_var([value, self.current_order])

    def JUMP(self, label):
        order = self.labels.get_value(label[1])
        self.current_order = order

    def JUMPIFEQ(self, label, symb1, symb2):
        first_value, second_value, var_frame, var_value = self.relation_operators(label, symb1, symb2, True)
        final_value = first_value == second_value
        if final_value:
            self.current_order = self.labels.get_value(label[1])

    def JUMPIFNEQ(self, label, symb1, symb2):
        first_value, second_value, var_frame, var_value = self.relation_operators(label, symb1, symb2, True)
        final_value = first_value == second_value
        if not final_value:
            self.current_order = self.labels.get_value(label[1])

    def EXIT(self, symb):
        first_frame, first_value = self.check_operand(('int', 'var'), symb)
        first_type, first_value = self.check_variable(first_frame, first_value, 'int')
        first_value = int(first_value)
        if first_value < 0 or first_value > 49:
            exit(57)
        else:
            exit(first_value)

    @staticmethod
    def DPRINT(symb):
        stderr.write('DPRINT: {}\n'.format(symb))

    def BREAK(self):
        stderr.write('ORDER: {}\n'.format(self.current_order))
        for key, value in self.frames.items():
            if value is None:
                stderr.write("{}: None\n".format(key))
                continue
            stderr.write("{}: {}\n".format(key, value.locals))


def main():
    argparser = argparse.ArgumentParser()
    argparser.add_argument('--source', dest='source_file', default=stdin, help='Path to XML file with source code')
    argparser.add_argument('--input', dest='input_file', default=stdin, help='Path to file with user input')
    args = argparser.parse_args()
    if args.source_file == stdin and args.input_file == stdin:
        stderr.write('Missing argument\n')
        exit(10)
    check_files((args.source_file, args.input_file))
    interpreter = Interpreter(args.source_file, args.input_file)
    interpreter.parse_src()
    interpreter.execute()


def check_files(args):
    for arg in args:
        if arg == stdin:
            continue
        if not os.path.isfile(arg):
            stderr.write('Argument is not file\n')
            exit(11)
        if not os.access(arg, os.R_OK):
            stderr.write('File is not accessible\n')
            exit(11)


if __name__ == '__main__':
    main()
