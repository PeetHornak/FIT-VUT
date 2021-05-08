import           Converter
import           Data.Char
import           Infix
import           Insert
import           Shunting

import           System.Environment             ( getArgs )
import           System.IO                      ( getContents
                                                , hFlush
                                                , putStr
                                                , putStrLn
                                                , readFile
                                                , stdout
                                                )

reToRka :: String -> IO ()
reToRka = putStr . show . convertRegex . transformRe

reRepresentation :: String -> IO ()
reRepresentation = putStr . toInfix . transformRe

transformRe :: String -> String
transformRe = shuntingYard . insertConcat . removeWhiteSpace

main :: IO ()
main = do
  (action, input) <- procArgs =<< getArgs
  action input

procArgs :: [String] -> IO (String -> IO (), String)
procArgs [x, y] = do
  input <- readFile y
  procParam x input
procArgs [x] = do
  input <- getContents
  procParam x input
procArgs x = error "Wrong arguments"

procParam :: String -> String -> IO (String -> IO (), String)
procParam x input = case x of
  "-t" -> return (reToRka, input)
  "-i" -> return (reRepresentation, input)
  _    -> error "Unknown option"

removeWhiteSpace :: [Char] -> [Char]
removeWhiteSpace = filter (not . isSpace)
