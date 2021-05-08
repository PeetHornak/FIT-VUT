module Shunting
  ( shuntingYard
  ) where

import           Helpers

import           Data.List


shuntingYard :: [Char] -> [Char]
shuntingYard tokens = shuntingYard' tokens [] []

shuntingYard' :: [Char] -> [Char] -> [Char] -> [Char]
-- reverse output if only output queue
shuntingYard' [] []         outQueue = reverse outQueue
-- add all operators to output queue if no input tokens
shuntingYard' [] (op : ops) outQueue = shuntingYard' [] ops (op : outQueue)
shuntingYard' (token : tokens) opStack outQueue
  |
  -- if the token is a regular expression then put it on the output queue
    isAlpha token = shuntingYard' tokens opStack (token : outQueue)
  |
  -- if token is iteration (unary) operator then put in on the output queue 
    isIteration token = shuntingYard' tokens opStack (token : outQueue)
  |
  -- if the token is binary operator
    isOperator token = case opStack of
        -- if operator stack is empty then add the operator to the stack
    []          -> shuntingYard' tokens (token : opStack) outQueue
    -- | if the operator stack isn't empty compare precedence with operator on top of opStack 
    (op2 : ops) -> if operatorPrecedence token <= operatorPrecedence op2
      then
                     -- Put operator on output queue if it has bigger precedence
           shuntingYard' (token : tokens) ops (op2 : outQueue)
                     -- Put token on opStack otherwise
      else shuntingYard' tokens (token : opStack) outQueue
  |
  -- if token is left bracket then push onto operator stack
    isLeftBracket token = shuntingYard' tokens (token : opStack) outQueue
  |
  -- if token is right bracket then check operator stack 
    isRightBracket token = case opStack of
        -- if top of operator stack is left bracket then just pop
    ('(' : ops) -> shuntingYard' tokens ops outQueue
    -- pop from operator stack and push into output queue till reach left bracket
    (op  : ops) -> shuntingYard' (token : tokens) ops (op : outQueue)
  | otherwise = error ("Not supported character: " ++ (return token :: String))

operatorPrecedence :: Char -> Int
operatorPrecedence '+' = 2
operatorPrecedence '.' = 3
operatorPrecedence _   = 0
