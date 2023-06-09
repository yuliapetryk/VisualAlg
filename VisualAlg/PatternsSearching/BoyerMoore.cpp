#include "BoyerMoore.h"
/// Searches for the pattern in the given text using the Boyer-Moore algorithm.
/// @param text The text to search in.
/// @param pattern The pattern to search for.
/// @returns The positions where the pattern is found.
void BoyerMoore::search(const QString& text, const QString& pattern)
{
    int n = text.length();
    int m = pattern.length();

    std::vector<int> badCharacter = computeBadCharacterTable(pattern);
    std::vector<int> goodSuffix = computeGoodSuffixTable(pattern);

    std::vector<int> positions;

    int i = 0;
    while (i <= n - m) {
        int j = m - 1;

        while (j >= 0 && pattern[j] == text[i + j])
            j--;

        if (j < 0) {
            positions.push_back(i);
            i += goodSuffix[0];
        } else {
            int shift1 = j - badCharacter[text[i + j].unicode()];
            int shift2 = 0;
            if (j < m - 1) {
                shift2 = goodSuffix[j + 1];
            }
            i += std::max(shift1, shift2);
        }
    }
    searchPositions = positions;
    emit searchCompleted(positions);
}
/// Computes the bad character table for the Boyer-Moore algorithm.
/// @param pattern The pattern to compute the table for.
/// @returns The bad character table.
std::vector<int> BoyerMoore::computeBadCharacterTable(const QString& pattern)
{
    std::vector<int> badCharacter(256, -1);

    for (int i = 0; i < pattern.length(); i++) {
        int c = pattern[i].unicode();
        badCharacter[c] = i;
    }

    return badCharacter;
}
/// Computes the good suffix table for the Boyer-Moore algorithm.
/// @param pattern The pattern to compute the table for.
/// @returns The good suffix table.
std::vector<int> BoyerMoore::computeGoodSuffixTable(const QString& pattern)
{
    int m = pattern.length();

    std::vector<int> suffixShift(m + 1, 0);
    std::vector<int> border(m + 1, 0);

    int i = m;
    int j = m + 1;
    border[i] = j;

    while (i > 0) {
        while (j <= m && pattern[i - 1] != pattern[j - 1]) {
            if (suffixShift[j] == 0) {
                suffixShift[j] = j - i;
            }
            j = border[j];
        }
        i--;
        j--;
        border[i] = j;
    }

    j = border[0];
    for (i = 0; i <= m; i++) {
        if (suffixShift[i] == 0) {
            suffixShift[i] = j;
        }
        if (i == j) {
            j = border[j];
        }
    }

    return suffixShift;
}
/// Returns the positions where the pattern is found in the text.
/// @returns The positions where the pattern is found.
std::vector<int>& BoyerMoore::getSearchPositions() {
    return  searchPositions;
}
