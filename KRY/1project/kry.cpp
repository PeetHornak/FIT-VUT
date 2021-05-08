#include <iostream>
#include <iterator>
#include <string>
#include <unordered_map>
#include <map>
#include <vector>
#include <algorithm>
#include <set>
#include <cmath>

typedef std::vector<int> positionList;
typedef std::vector<std::string> textColumns;
typedef std::unordered_map<std::string, positionList> nGrams;
typedef std::map<int, int> occurrences;
typedef std::map<char, int> charFrequency;


std::string read_input() {
    std::string text;
    char c;
    while (std::cin.get(c)) {
        if (std::isalpha(c)) {
            if (std::isupper(c)) {
                c = std::tolower(c);
            }
            text += c;
        }
    }
    return text;
}


void find_ngrams(std::string *text, nGrams *n_grams, int n_gram_length) {
    positionList word;

    bool found_multiple = false;

    for (unsigned int i = 0; i < text->size() - n_gram_length; i++) {
        std::string sub = text->substr(i, n_gram_length);
        word = (*n_grams)[sub];
        word.push_back(i);
        if (word.size() > 1) {
            found_multiple = true;
        }
        (*n_grams)[sub] = word;
    }

    if (!found_multiple && n_gram_length > 3) {
        find_ngrams(text, n_grams, n_gram_length - 1);
    }

}

void ngrams_to_occurences(nGrams *n_grams, occurrences *occ) {
    for (auto const &item : *n_grams) {
        for (std::vector<int>::size_type i = 0; i < item.second.size() - 1; i++) {
            int distance = item.second[i + 1] - item.second[i];
            (*occ)[distance] += 1;
        }
    }
}

void occurrences_to_vector(occurrences *occ, std::vector<int> *occ_count) {
    for (auto const &item : *occ) {
        occ_count->push_back(item.first);
    }
    std::sort(occ_count->begin(), occ_count->end(), [occ](int a, int b) { return (*occ)[a] > (*occ)[b]; });
}

std::set<int> find_factors(int number, std::set<int> *previous_factors) {

    std::set<int> current_set;
    std::set<int> intersect;

    for (int i = 1; i * i <= number; i++) {
        if (number % i == 0) {
            if (i > 2)
                current_set.insert(i);
            current_set.insert(number / i);
        }
    }

    if (previous_factors->empty())
        return current_set;
    else {
        std::set_intersection(previous_factors->begin(), previous_factors->end(), current_set.begin(),
                              current_set.end(),
                              std::inserter(intersect, intersect.begin()));
        if (intersect.empty())
            return *previous_factors;

        return intersect;
    }
}


int kasiski_test(std::string *text) {
    nGrams n_grams;
    occurrences occ;
    std::vector<int> occ_count;
    std::set<int> factors;

    find_ngrams(text, &n_grams, 5);
    ngrams_to_occurences(&n_grams, &occ);

    occurrences_to_vector(&occ, &occ_count);

    int i = 0;
    for (auto const &item : occ_count) {
        if (i > 10) {
            break;
        }
        factors = find_factors(item, &factors);
        i++;
    }

    if (factors.empty())
        return 0;

    return *factors.rbegin();
}

float calculate_coincidence(std::string *text) {
    double coincidence_index = 0;
    const std::string alphabet = "abcdefghijklmnopqrstuvwxyz";


    double len = text->length();
    for (auto const &character : alphabet) {
        int c = count(text->begin(), text->end(), character);
        coincidence_index += c * (c - 1) / (len * (len - 1));
    }

    coincidence_index = coincidence_index;
    return coincidence_index;
}

float friedman_test(std::string *text) {
    charFrequency char_freq;
    const float kr = 1.0 / 26.0;
    const float k_eng = 0.0656;

    float coincidence_index = calculate_coincidence(text);

    return (k_eng - kr) / (coincidence_index - kr);
}

textColumns split_to_columns(std::string *text, int number) {
    textColumns text_columns(number);
    for (unsigned int i = 0; i < text->size(); i++) {
        text_columns[i % number] += (*text)[i];
    }

    return text_columns;
}

int guess_key_length(std::string *text, textColumns *final_columns) {
    const float k_eng = 0.0656;
    int i = 3;
    while (true) {
        float average = 0;
        textColumns text_columns = split_to_columns(text, i);
        for (auto &column : text_columns) {
            average += calculate_coincidence(&column);
        }
        average = average / text_columns.size();
        if (fabs(average - k_eng) < 0.01) {
            *final_columns = text_columns;
            return i;
        }
        i++;
    }
}

std::string guess_key(std::string *text, int key_len) {

    textColumns text_columns = split_to_columns(text, key_len);
    const std::string alphabet = "abcdefghijklmnopqrstuvwxyz";
    const float k_eng = 0.0656;
    const float probability[26] = {
            0.082,
            0.015,
            0.028,
            0.043,
            0.13,
            0.022,
            0.02,
            0.061,
            0.07,
            0.0015,
            0.0077,
            0.04,
            0.024,
            0.067,
            0.075,
            0.019,
            0.00095,
            0.06,
            0.063,
            0.091,
            0.028,
            0.0098,
            0.024,
            0.0015,
            0.02,
            0.00074
    };

    std::string key;

    for (auto const &column : text_columns) {
        float closest = 1;
        int closest_index = 0;
        std::map<char, int> frequencies;

        for (auto const &c : alphabet) {
            int freq = count(column.begin(), column.end(), c);
            frequencies[c] = freq;
        }


        for (int shift = 0; shift < 26; shift++) {
            float sum = 0;
            int len = column.length();

            for (int i = 0; i < 26; i++) {
                sum += probability[i] * frequencies[alphabet[(i + shift) % 26]] / len;
            }
            float distance = fabs(k_eng - sum);
            if (distance < closest) {
                closest = distance;
                closest_index = shift;
            }
        }
        key += alphabet[closest_index];
    }

    return key;
}

int main() {
    std::string text;
    std::vector<std::string> column_split_text;

    text = read_input();

    float friedman_key_length = friedman_test(&text);
    std::cout << friedman_key_length << ';';

    int kasiski_key_length = kasiski_test(&text);
    std::cout << kasiski_key_length << ';';

    int guessed_key_length = guess_key_length(&text, &column_split_text);
    std::cout << guessed_key_length << ';';

    int key_len = kasiski_key_length;
    if (key_len == 0)
        key_len = guessed_key_length;

    std::string key = guess_key(&text, key_len);
    std::cout << key;

    std::cout << std::endl;

    return 0;
}