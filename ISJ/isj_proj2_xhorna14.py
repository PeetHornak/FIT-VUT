#!/usr/bin/env python3

# ukol za 2 body
def she_says_he_says(she_says):
    """Replaces y/i, removes spaces, returns reversed 

    >>> she_says_he_says('ma rymu')
    'umiram'
    """

    phonetic_she_says = she_says.replace('y', 'i')                             # vase reseni
    compact = phonetic_she_says.replace(' ', '')                              # vase reseni
    he_says = compact[::-1]                                        # vase reseni
    return he_says


# ukol za 3 body
def solfege(title_hymn):
    """Partitions the input string to (an optional) title, ': ', and the hymn,
       takes a sublist starting from the first string, skipping always two 
       other strings, and ending 3 strings from the end, returns the result 
       as a string with ', ' as a separator

    >>> solfege('Hymn of St. John: Ut queant laxis re sonare fibris mi ra gestorum fa muli tuorum sol ve polluti la bii reatum Sancte Iohannes')
    'Ut, re, mi, fa, sol, la'

    >>> solfege('Ut queant laxis re sonare fibris mi ra gestorum fa muli tuorum sol ve polluti la bii reatum Sancte Iohannes')
    'Ut, re, mi, fa, sol, la'
    """

    # the input string partitioned to the title (if given) and the actual hymn
    possible_title, hymn = title_hymn.split(': ') if ': ' in title_hymn else ('', title_hymn)  # vase reseni

    # the hymn as a list of strings separated by ' '
    hymn_list = hymn.split(' ')                                               # vase reseni
    
    # skipping always two strings, and ending 3 strings from the end
    skip2 = hymn_list[:-3:3]                                                   # vase reseni

    # the skip2 list as a string, ', ' as a separator
    skip2_str = ", ".join(skip2)                                               # vase reseni

    return skip2_str


if __name__ == "__main__":
    import doctest
    doctest.testmod()
    pass
