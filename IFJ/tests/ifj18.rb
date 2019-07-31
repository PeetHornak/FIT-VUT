#!/bin/ruby
# Zajisteni zakladni kompatibility IFJ18->Ruby @ Merlin
def inputs()
  STDIN.gets
end

def inputi()
  STDIN.gets.to_i
end

def inputf()
  STDIN.gets.to_f
end

def length(s)
  s.length
end

def substr(s, i, n)
  s.slice(i, n)
end

def ord(s, i)
  s[i].ord
end

def chr(i)
  i.chr
end

def print(*more)
    for arg in more
        if arg.class == Float then
            Kernel.print("%a" % arg)
        else
            Kernel.print(arg)
        end
    end
    nil
end

# Zde bude nasledovat program jazyka IFJ18

