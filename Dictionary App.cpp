#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <limits>

using namespace std;

// Structure to store a word and its meaning
struct Word {
    string term;
    string meaning;
};

// Global dictionary vector
vector<Word> dictionary;

// File names
const string DATA_FILE = "dictionary.txt";
const string BACKUP_FILE = "backup_dictionary.txt";

// Function to clear invalid input
void clearInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// Function to load dictionary from file
void loadDictionary() {
    dictionary.clear();
    ifstream file(DATA_FILE);
    if (!file.is_open()) return;
    string line;
    while (getline(file, line)) {
        size_t delim = line.find('|');
        if (delim != string::npos) {
            Word w;
            w.term = line.substr(0, delim);
            w.meaning = line.substr(delim + 1);
            dictionary.push_back(w);
        }
    }
    file.close();
}

// Function to save dictionary to file
void saveDictionary() {
    ofstream file(DATA_FILE);
    for (auto &w : dictionary) {
        file << w.term << "|" << w.meaning << "\n";
    }
    file.close();
}

// Backup dictionary
void backupDictionary() {
    ofstream backup(BACKUP_FILE);
    for (auto &w : dictionary) {
        backup << w.term << "|" << w.meaning << "\n";
    }
    backup.close();
    cout << "Backup created successfully as '" << BACKUP_FILE << "'\n";
}

// Restore dictionary from backup
void restoreDictionary() {
    ifstream backup(BACKUP_FILE);
    if (!backup.is_open()) {
        cout << "No backup found.\n";
        return;
    }
    dictionary.clear();
    string line;
    while (getline(backup, line)) {
        size_t delim = line.find('|');
        if (delim != string::npos) {
            Word w;
            w.term = line.substr(0, delim);
            w.meaning = line.substr(delim + 1);
            dictionary.push_back(w);
        }
    }
    backup.close();
    saveDictionary();
    cout << "Dictionary restored from backup successfully.\n";
}

// Function to check if word exists
int findWordIndex(const string &term) {
    for (int i = 0; i < dictionary.size(); i++) {
        if (dictionary[i].term == term)
            return i;
    }
    return -1;
}

// Function to add a word
void addWord() {
    string term, meaning;
    cout << "Enter word: ";
    getline(cin, term);
    if (term.empty()) {
        cout << "Word cannot be empty!\n";
        return;
    }
    if (findWordIndex(term) != -1) {
        cout << "Word already exists!\n";
        return;
    }
    cout << "Enter meaning: ";
    getline(cin, meaning);
    if (meaning.empty()) {
        cout << "Meaning cannot be empty!\n";
        return;
    }
    dictionary.push_back({term, meaning});
    saveDictionary();
    cout << "Word added successfully.\n";
}

// Function to update a word
void updateWord() {
    string term;
    cout << "Enter word to update: ";
    getline(cin, term);
    int idx = findWordIndex(term);
    if (idx == -1) {
        cout << "Word not found!\n";
        return;
    }
    string newMeaning;
    cout << "Enter new meaning: ";
    getline(cin, newMeaning);
    if (newMeaning.empty()) {
        cout << "Meaning cannot be empty!\n";
        return;
    }
    dictionary[idx].meaning = newMeaning;
    saveDictionary();
    cout << "Word updated successfully.\n";
}

// Function to delete a word
void deleteWord() {
    string term;
    cout << "Enter word to delete: ";
    getline(cin, term);
    int idx = findWordIndex(term);
    if (idx == -1) {
        cout << "Word not found!\n";
        return;
    }
    dictionary.erase(dictionary.begin() + idx);
    saveDictionary();
    cout << "Word deleted successfully.\n";
}

// Function to search a word
void searchWord() {
    string term;
    cout << "Enter word to search: ";
    getline(cin, term);
    int idx = findWordIndex(term);
    if (idx == -1) {
        cout << "Word not found!\n";
        return;
    }
    cout << dictionary[idx].term << " : " << dictionary[idx].meaning << "\n";
}

// Partial search
void partialSearch() {
    string term;
    cout << "Enter letters to search: ";
    getline(cin, term);
    bool found = false;
    for (auto &w : dictionary) {
        if (w.term.find(term) != string::npos) {
            cout << w.term << " : " << w.meaning << "\n";
            found = true;
        }
    }
    if (!found) cout << "No matching words found.\n";
}

// Display all words alphabetically
void displayAllWords() {
    if (dictionary.empty()) {
        cout << "Dictionary is empty!\n";
        return;
    }
    sort(dictionary.begin(), dictionary.end(), [](Word a, Word b){
        return a.term < b.term;
    });
    cout << "All Words:\n";
    for (auto &w : dictionary) {
        cout << w.term << " : " << w.meaning << "\n";
    }
}

// Word count
void wordCount() {
    cout << "Total words in dictionary: " << dictionary.size() << "\n";
}

// Main menu
void menu() {
    int choice;
    do {
        cout << "\n===== DICTIONARY APP =====\n";
        cout << "1. Add Word\n";
        cout << "2. Update Word\n";
        cout << "3. Delete Word\n";
        cout << "4. Search Word\n";
        cout << "5. Partial Search\n";
        cout << "6. Display All Words\n";
        cout << "7. Word Count\n";
        cout << "8. Backup Dictionary\n";
        cout << "9. Restore Dictionary\n";
        cout << "0. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;
        clearInput();

        switch (choice) {
            case 1: addWord(); break;
            case 2: updateWord(); break;
            case 3: deleteWord(); break;
            case 4: searchWord(); break;
            case 5: partialSearch(); break;
            case 6: displayAllWords(); break;
            case 7: wordCount(); break;
            case 8: backupDictionary(); break;
            case 9: restoreDictionary(); break;
            case 0: cout << "Exiting...\n"; break;
            default: cout << "Invalid choice! Try again.\n";
        }
    } while (choice != 0);
}

// Main function
int main() {
    loadDictionary(); // Load words at start
    menu();
    return 0;
}
