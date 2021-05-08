module Converter
  ( convertRegex
  , Automata
  ) where

import           Data.List

import           Helpers

data Rule = Rule
  { firstState  :: Int
  , character   :: [Char]
  , secondState :: Int
  }

instance Show Rule where
  show (Rule first char second) =
    intercalate "," [show first, char, show second]

data Automata = Automata
  { state      :: [Int]
  , alphabet   :: [Char]
  , initState  :: Int
  , finalState :: Int
  , rules      :: [Rule]
  }

instance Show Automata where
  show (Automata states alph init final rules) =
    unlines $ [showStates states, alph, show init, show final] ++ showedRules
   where
    showStates  = intercalate "," . map show
    showedRules = map show rules


convertRegex :: [Char] -> Automata
--convertRegex x = error x
convertRegex x = convertRegex' x []

convertRegex' :: [Char] -> [Automata] -> Automata
convertRegex' [] [out] = out
convertRegex' [] out   = error "Invalid regex"
convertRegex' (x : xs) out
  | isAlpha x         = convertRegex' xs (handleAlpha [x] out)
  | isIteration x     = convertRegex' xs (handleIteration out)
  | isConcatenation x = convertRegex' xs (handleConcatenation out)
  | isUnification x   = convertRegex' xs (handleUnification out)

handleAlpha :: [Char] -> [Automata] -> [Automata]
handleAlpha "#" out = handleAlpha [] out
handleAlpha x   out = newAutomata : out
 where
  newAutomata = Automata [firstNew, secondNew]
                         x
                         firstNew
                         secondNew
                         [Rule firstNew x secondNew]
  firstNew  = getNumberOfStates out
  secondNew = firstNew + 1

handleIteration :: [Automata] -> [Automata]
handleIteration (stackHead : out) = newAutomata : out
 where
  newAutomata =
    Automata newStates (alphabet stackHead) firstNew secondNew newRules
  firstNew  = getNumberOfStates [stackHead]
  secondNew = firstNew + 1
  newStates = [firstNew, secondNew] ++ state stackHead
  newRules =
    [ Rule firstNew               "" (initState stackHead)
      , Rule (finalState stackHead) "" (initState stackHead)
      , Rule (finalState stackHead) "" secondNew
      , Rule firstNew               "" secondNew
      ]
      ++ rules stackHead

handleUnification :: [Automata] -> [Automata]
handleUnification (a2 : a1 : out) = newAutomata : out
 where
  newAutomata = Automata newStates newAlphabet firstNew secondNew newRules
  firstNew    = getNumberOfStates [a2]
  secondNew   = firstNew + 1
  newStates   = [firstNew, secondNew] ++ state a1 ++ state a2
  newAlphabet = alphabet a1 `union` alphabet a2
  newRules =
    [ Rule firstNew        "" (initState a1)
      , Rule firstNew        "" (initState a2)
      , Rule (finalState a1) "" secondNew
      , Rule (finalState a2) "" secondNew
      ]
      ++ rules a1
      ++ rules a2

handleConcatenation :: [Automata] -> [Automata]
handleConcatenation (a2 : a1 : out) = newAutomata : out
 where
  newAutomata =
    Automata newStates newAlphabet (initState a1) (finalState a2) newRules
  newStates   = state a1 ++ delete (initState a2) (state a2)
  newAlphabet = alphabet a1 `union` alphabet a2
  newRules =
    rules a1 ++ replaceCombinedState (rules a2) (initState a2) (finalState a1)

replaceCombinedState :: [Rule] -> Int -> Int -> [Rule]
replaceCombinedState rules s1 s2 = replaceCombinedState' rules s1 s2 []

replaceCombinedState' :: [Rule] -> Int -> Int -> [Rule] -> [Rule]
replaceCombinedState' [] _ _ out = out
replaceCombinedState' (rule : rules) s1 s2 out
  | firstState rule == s1 = replaceCombinedState'
    rules
    s1
    s2
    (Rule s2 ruleCharacter ruleSecondState : out)
  | otherwise = replaceCombinedState' rules s1 s2 (rule : out)
 where
  ruleCharacter   = character rule
  ruleSecondState = secondState rule

getNumberOfStates :: [Automata] -> Int
getNumberOfStates []      = 0
getNumberOfStates (x : _) = maximum (state x) + 1
