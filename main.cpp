#include <iostream>
#include <cstring>
#include <random>

void ClearBadInput() {
    std::cin.clear();
    std::cin.ignore(10000, '\n');
}

bool getIntOnce(const char* request, int& res) {
    static constexpr int int_buf_size = 13;
    char buf[int_buf_size];

    std::cout << request << '\n';
    std::cin.getline(buf, int_buf_size);

    if (std::cin.fail()) {
        ClearBadInput();
        return false;
    }

    int i = 0;
    bool neg = false;
    if (buf[0] == '-') { neg = true; i = 1; }

    if (buf[i] == '\0') {
        return false;
    }

    bool ok = true;
        for (int j = i; buf[j] != '\0'; ++j) {
        if (buf[j] < '0' || buf[j] > '9') { ok = false; break; }
    }
    if (!ok) {
        return false;
    }

    long val = 0;
    const long long limit = neg ? 2147483647LL + 1 : 2147483647LL;
    for (; buf[i] != '\0'; ++i) {
        if (val > (limit - (buf[i] - '0')) / 10) { ok = false; break; }
        val = val * 10 + (buf[i] - '0');
    }
    if (!ok) {
        return false;
    }

    res = neg ? (int)(-val) : (int)val;
    return true;
}

int len(const char* word) {
    int len = 0;
    while (*word != '\0') {++len; ++word;}
    return len;
}

void encryptWord(const char* original, char* encrypted, int key)
{
    while (*original != '\0')
    {
        char ch = *original;

        if (ch >= 'a' && ch <= 'z')
        {
            *encrypted = static_cast<char>('a' + (ch - 'a' + key) % 26);
        }
        else if (ch >= 'A' && ch <= 'Z')
        {
            *encrypted = static_cast<char>('A' + (ch - 'A' + key) % 26);
        }
        else
        {
            *encrypted = ch;
        }

        ++original;
        ++encrypted;
    }

    *encrypted = '\0';
}

void revealOneLetter(const char* original, char* encrypted)
{
    while (*original != '\0' && *encrypted != '\0')
    {
        if (*encrypted != *original)
        {
            *encrypted = *original;
            return;
        }

        ++original;
        ++encrypted;
    }
}

bool checkAnswer(const char* userGuess, const char* original, int& attempts)
{
    if (std::strcmp(userGuess, original) == 0)
    {
        return true;
    }

    --attempts;
    return false;
}

int main()
{
    static constexpr int godNumber = 100;

    static char secret[godNumber];
    char encrypted[godNumber];
    char masked[godNumber];
    char guess[godNumber];
    int key = 0;
    int attempts = 3;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(1, 5);

    char enter[godNumber];
    bool bad = true;

    while (bad) {
        std::cout << "Wrote secret word\n";
        std::cin.getline(enter, sizeof(enter));
        if (std::cin.fail()) ClearBadInput();
        bad = false;
        if (std::strcmp(enter, "hint") == 0) {
            std::cout << "=_=\n\n";
            bad = true;
            continue;
        }
        for (int i = 0; i < 4; ++i) {
            if (enter[i] == '\0') {
                bad = true;
                break;
            }
        }
    }

    const int hit = len(enter);

    std::strcpy(secret, enter);

    while (!getIntOnce("Wrote shift (0 to random) ", key) || (key % 26 == 0 && key != 0)) {}
    if (key == 0) { key = dist(gen);}
    key = ((key % 26) + 26) %  26;

    std::cout << "\n\n\n\n\n\n\n\n\n\n\n";

    encryptWord(secret, encrypted, key);

    {
        const char* p = secret;
        char* m = masked;
        while (*p != '\0') {
            if ((*p >= 'a' && *p <= 'z') || (*p >= 'A' && *p <= 'Z')) *m = '*';
            else *m = *p;
            ++p;
            ++m;
        }
        *m = '\0';
    }

    std::cout << "Encrypted word: " << encrypted << "\n";
    std::cout << "Input word or hint.\n\n";

    bool hintAvailable = false;

    while (attempts > 0){
        std::cout << "\nEncrypted: " << encrypted << "\n";
        std::cout << "Mask:      " << masked    << "\n";
        std::cout << "You have: " << attempts << " attempts" << "\n";

        std::cin.getline(guess, sizeof(guess));

        if (std::strcmp(guess, "hint") == 0)
        {
            if (!hintAvailable) {
                std::cout << "Hint is not available. \n\n";
                continue;
            }
            revealOneLetter(secret, masked);
            hintAvailable = false;
            std::cout << "Mask: " << masked << "\n\n";
            continue;
        }
        if (len(guess) != hit) {
            std::cout << "You need to input " << hit << " symbols.\n";
            continue;
        }
        if (checkAnswer(guess, secret, attempts))
        {
            std::cout << "Yeah, it is " << secret << "\n";
            return 0;
        }
        else {
            if (attempts > 0) {
                std::cout << "Ne.\n";
                hintAvailable = true;
                std::cout << "Hint is now available (once).\n";
            }
            if (!hintAvailable) {
                std::cout << "Hint is now available (once).\n";
            }
        }
    }

    std::cout << "Nah, the right word was " << secret << "\n";
    return 0;
}