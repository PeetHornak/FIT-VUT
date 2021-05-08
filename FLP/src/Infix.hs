module Infix
  ( toInfix
  ) where

import           Helpers

toInfix :: String -> String
toInfix tokens = toInfix' tokens []

toInfix' :: String -> [String] -> String
toInfix' [] []         = []
toInfix' [] alphaStack = head alphaStack
toInfix' (token : tokens) alphaStack
  |
-- put token on stack if it is part of alphabet
    isAlpha token = toInfix' tokens ([token] : alphaStack)
  |
-- Append * to token on top of stack
    isIteration token = case alphaStack of
    []           -> error "Wrong postfix regex"
    (alp : alps) -> toInfix' tokens ((alp ++ [token]) : alps)
  |
-- Combine top two tokens on stack using current operator, add brackets
    isOperator token = case alphaStack of
    []    -> error "Wrong postfix regex"
    [alp] -> error "Wrong postfix regex"
    (alp1 : alp2 : alps) ->
      toInfix' tokens (("(" ++ alp2 ++ checkToken token ++ alp1 ++ ")") : alps)

checkToken :: Char -> [Char]
-- Remove '.' operator as it is used only internally
checkToken '.'   = []
checkToken token = [token]
