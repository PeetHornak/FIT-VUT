#!/usr/bin/env python3

# ukol za 2 body
def first_odd_or_even(numbers):
    """Returns 0 if there is the same number of even numbers and odd numbers
       in the input list of ints, or there are only odd or only even numbers.
       Returns the first odd number in the input list if the list has more even
       numbers.
       Returns the first even number in the input list if the list has more odd 
       numbers.

    >>> first_odd_or_even([2,4,2,3,6])
    3
    >>> first_odd_or_even([3,5,4])
    4
    >>> first_odd_or_even([2,4,3,5])
    0
    >>> first_odd_or_even([2,4])
    0
    >>> first_odd_or_even([3])
    0
    """
    odd = 0
    odd_number = None
    even_number = None
    for n in numbers:
        is_odd = 1 if n % 2 else 0
        odd += is_odd
        if odd_number is None and is_odd == 1:
            odd_number = n
        if even_number is None and is_odd == 0:
            even_number = n

    if odd == 0 or odd == len(numbers) or odd == len(numbers) / 2:
        return 0

    even = len(numbers) - odd
    if odd > even:
        return even_number
    
    return odd_number
    

# ukol za 3 body
def to_pilot_alpha(word):
    """Returns a list of pilot alpha codes corresponding to the input word

    >>> to_pilot_alpha('Smrz')
    ['Sierra', 'Mike', 'Romeo', 'Zulu']
    """

    pilot_alpha = ['Alfa', 'Bravo', 'Charlie', 'Delta', 'Echo', 'Foxtrot',
        'Golf', 'Hotel', 'India', 'Juliett', 'Kilo', 'Lima', 'Mike',
        'November', 'Oscar', 'Papa', 'Quebec', 'Romeo', 'Sierra', 'Tango',
        'Uniform', 'Victor', 'Whiskey', 'Xray', 'Yankee', 'Zulu']

    pilot_alpha_list = []
    
    word = word.upper()
    for c in word:
        index = ord(c) - 65
        pilot_alpha_list.append(pilot_alpha[index])

    return pilot_alpha_list


if __name__ == "__main__":
    import doctest
    doctest.testmod()
