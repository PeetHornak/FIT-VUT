module Insert
  ( insertConcat
  ) where

import           Helpers

insertConcat :: [Char] -> [Char]
insertConcat x = insertConcat' x []

insertConcat' :: [Char] -> [Char] -> [Char]
insertConcat' []    out = reverse out
insertConcat' [str] out = insertConcat' [] (str : out)
insertConcat' (s1 : s2 : strs) out
  | toConcat  = insertConcat' (s2 : strs) ('.' : s1 : out)
  | otherwise = insertConcat' (s2 : strs) (s1 : out)
  where toConcat = shouldBeConcated s1 s2

shouldBeConcated :: Char -> Char -> Bool
shouldBeConcated s1 s2 | isAlpha s1 && isAlpha s2           = True
                       | isAlpha s1 && isLeftBracket s2     = True
                       | isRightBracket s1 && isAlpha s2    = True
                       | isIteration s1 && isAlpha s2       = True
                       | isIteration s1 && isLeftBracket s2 = True
                       | otherwise                          = False

