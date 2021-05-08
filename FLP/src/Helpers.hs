module Helpers
  ( isLeftBracket
  , isRightBracket
  , isIteration
  , isConcatenation
  , isUnification
  , isOperator
  , isAlpha
  ) where

import qualified Data.Char                     as D

isLeftBracket :: Char -> Bool
isLeftBracket '(' = True
isLeftBracket _   = False

isRightBracket :: Char -> Bool
isRightBracket ')' = True
isRightBracket _   = False

isIteration :: Char -> Bool
isIteration '*' = True
isIteration _   = False

isConcatenation :: Char -> Bool
isConcatenation '.' = True
isConcatenation _   = False

isUnification :: Char -> Bool
isUnification '+' = True
isUnification _   = False

isOperator :: Char -> Bool
isOperator x = isUnification x || isConcatenation x

isAlpha :: Char -> Bool
isAlpha '#' = True
isAlpha x   = D.isLower x
