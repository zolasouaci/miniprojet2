#include <iostream>
#include <vector>
#include <fstream>
using namespace std;

const string TEMP_FILE = ".ignore";


vector<string> splitS (const string &s, const string &delimiter);
void readString (string &input);
string updata (int &choice);



class Personne {
    public:
        int id;
        string nom;
        int zoneGeo;
        string formatForFile() {return to_string(id) + " " + nom + " " + to_string(zoneGeo);}
        string formatForUser(const string &temp) {return temp + "\nid:" + to_string(id) + " name:" + nom + " zone:" + to_string(zoneGeo);}
};

class Recommande {
    public:
        int id;
        string type;
        int idFacteur;
        int idDestinataire;
        Recommande(int x, string s, int f, int d) {id = x; type = s; idFacteur = f; idDestinataire = d;}
        string formatForFile() {return to_string(id) + " " + type + " " + to_string(idFacteur) + " " + to_string(idDestinataire);}
        string formatForUser() {return "RECOMMANDE\nid:" + to_string(id) + " type:" + type + " idFacteur:" + to_string(idFacteur) + " idDestinataire:" + to_string(idDestinataire);}
};

class Facteur : public Personne {
    public:
        Facteur(int x, string n, int z) {id = x; nom = n; zoneGeo = z;}
        
};

class Habitant : public Personne {
    public:
        Habitant(int x, string n, int z) {id = x; nom = n; zoneGeo = z;}
};

int main() {
    string filename, line, password;
    int deleteLine, choice;


    cout << "Give database name:" << endl;
    readString(filename);
    filename = filename + ".txt";

    while (true) { // While loop for looping program and not closing when done

        cout << "Choose action (0:Add line, 1:delete data, 2:read):" << endl;
        cin >> choice;

        if (choice == 0) { //add 

            int classType;
            string output;

            cout << "What to add? (0:Facteur, 1:Habitant, 2:Recommande):" << endl;
            cin >> choice;

            if (choice != 0 && choice != 1 && choice != 2) {

                cout << "invalid\n" << endl;
                return 1;

            }

            classType = choice;

            // Format the user input into data to put in file
            output = to_string(classType) + " " + updata(choice);

            //open file
            ofstream fileOut;
            fileOut.open(filename, ios_base::app);

            // Put data to file 
            fileOut << output << endl;

            fileOut.close();

        } else if (choice == 1) { //remove
            int i=0;
            bool successfullyDeleted = false;

            cout << "Give line number to delete:" << endl;
            cin >> deleteLine;
            deleteLine -= 1;

            ifstream fileIn;
            fileIn.open(filename);
            ofstream temp;
            temp.open(TEMP_FILE);

            while (getline(fileIn, line)) {
                // Write all lines to temp except the line marked for removing
                if (i != deleteLine) {
                    temp << line << endl;
                } else successfullyDeleted = true;
                i++;
            }
            temp.close();
            fileIn.close();

            // Replace original file with the temp one
            const char * p = filename.c_str();
            remove(p);
            rename(TEMP_FILE.c_str(), p);

            if (successfullyDeleted) {
                cout << "Successfully deleted data  " + to_string(deleteLine + 1) + "\n" << endl;
            } else cout << "line doesn't exist" << endl;

        } else if (choice == 2) { //read
            int i=1;

            ifstream fileIn;
            fileIn.open(filename);

            // Loop through file line by line
            while (getline(fileIn, line)) {

                vector<string> vector_input;
                string toShow;
                int classType;

                // Split the line into different variables then format for output
                vector_input = splitS(line, " ");
                classType = stoi(vector_input[0]);
                if (classType == 0) {

                    Facteur fac(stoi(vector_input[1]),vector_input[2],stoi(vector_input[3]));
                    toShow = fac.formatForUser("FACTEUR");

                } else if (classType == 1) {

                    Habitant hab(stoi(vector_input[1]),vector_input[2],stoi(vector_input[3]));
                    toShow = hab.formatForUser("HABITANT");

                } else if (classType == 2) {

                    Recommande reco(stoi(vector_input[1]),vector_input[2],stoi(vector_input[3]),stoi(vector_input[4]));
                    toShow = reco.formatForUser();
                }
                cout << to_string(i) + ":\n" + toShow + "\n" << endl;
                i++;
            }

            fileIn.close();

        } else { // User is actually retarded

            cout << " can't read\n" << endl;
            return 1;

        }
   
    }

    return 0;
}



// Split a string into strings 
vector<string> splitS(const string &s, const string &delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    string token;
    vector<string> res;

    while ((pos_end = s.find (delimiter, pos_start)) != string::npos) {
        token = s.substr (pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back (token);
    }

    res.push_back (s.substr (pos_start));
    return res;
}

//read fct
void readString(string &input) {
    getline(cin, input);
    if (input == "") {
        readString(input);
    }
}



string updata(int &choice) {

    string text_input;
    vector<string> vector_input;

    if (choice == 0 || choice == 1) {

        Personne personne;
        cout << "Give <id> <name> <zone geo>:" << endl;
        readString(text_input);
        vector_input = splitS(text_input, " ");

        if (choice == 0) {
            personne = Facteur(stoi(vector_input[0]),vector_input[1],stoi(vector_input[2]));
            cout << "Successfully added:\n\n" + personne.formatForUser("FACTEUR") + "\n" << endl;
        }
        if (choice == 1) {
            personne = Habitant(stoi(vector_input[0]),vector_input[1],stoi(vector_input[2]));
            cout << "Successfully added:\n\n" + personne.formatForUser("HABITANT") + "\n" << endl;
        }

        return personne.formatForFile();

    } else if (choice == 2) {

        string type[2] = {"lettre", "colis"};
        cout << "type? (0:lettre, 1:colis)" << endl;
        cin >> choice;
        cout << "Give <id> <idFacteur> <idDestinataire>" << endl;
        readString(text_input);
        vector_input = splitS(text_input, " ");

        Recommande reco(stoi(vector_input[0]),type[choice],stoi(vector_input[1]),stoi(vector_input[2]));

        cout << "Successfully added:\n\n" + reco.formatForUser() + "\n" << endl;

        return reco.formatForFile();
    }

    return 0;
}
////.............WRONG CHOICE = WASTE OF TIME = ENCRYPTION DON'T WORK

/*AES::AES(const AESKeyLength keyLength) {
  switch (keyLength) {
    case AESKeyLength::AES_128:
      this->Nk = 4;
      this->Nr = 10;
      break;
    case AESKeyLength::AES_192:
      this->Nk = 6;
      this->Nr = 12;
      break;
    case AESKeyLength::AES_256:
      this->Nk = 8;
      this->Nr = 14;
      break;
  }
}

unsigned char *AES::EncryptECB(const unsigned char in[], unsigned int inLen,
                               const unsigned char key[]) {
  CheckLength(inLen);
  unsigned char *out = new unsigned char[inLen];
  unsigned char *roundKeys = new unsigned char[4 * Nb * (Nr + 1)];
  KeyExpansion(key, roundKeys);
  for (unsigned int i = 0; i < inLen; i += blockBytesLen) {
    EncryptBlock(in + i, out + i, roundKeys);
  }

  delete[] roundKeys;

  return out;
}

unsigned char *AES::DecryptECB(const unsigned char in[], unsigned int inLen,
                               const unsigned char key[]) {
  CheckLength(inLen);
  unsigned char *out = new unsigned char[inLen];
  unsigned char *roundKeys = new unsigned char[4 * Nb * (Nr + 1)];
  KeyExpansion(key, roundKeys);
  for (unsigned int i = 0; i < inLen; i += blockBytesLen) {
    DecryptBlock(in + i, out + i, roundKeys);
  }

  delete[] roundKeys;

  return out;
}

unsigned char *AES::EncryptCBC(const unsigned char in[], unsigned int inLen,
                               const unsigned char key[],
                               const unsigned char *iv) {
  CheckLength(inLen);
  unsigned char *out = new unsigned char[inLen];
  unsigned char block[blockBytesLen];
  unsigned char *roundKeys = new unsigned char[4 * Nb * (Nr + 1)];
  KeyExpansion(key, roundKeys);
  memcpy(block, iv, blockBytesLen);
  for (unsigned int i = 0; i < inLen; i += blockBytesLen) {
    XorBlocks(block, in + i, block, blockBytesLen);
    EncryptBlock(block, out + i, roundKeys);
    memcpy(block, out + i, blockBytesLen);
  }

  delete[] roundKeys;

  return out;
}

unsigned char *AES::DecryptCBC(const unsigned char in[], unsigned int inLen,
                               const unsigned char key[],
                               const unsigned char *iv) {
  CheckLength(inLen);
  unsigned char *out = new unsigned char[inLen];
  unsigned char block[blockBytesLen];
  unsigned char *roundKeys = new unsigned char[4 * Nb * (Nr + 1)];
  KeyExpansion(key, roundKeys);
  memcpy(block, iv, blockBytesLen);
  for (unsigned int i = 0; i < inLen; i += blockBytesLen) {
    DecryptBlock(in + i, out + i, roundKeys);
    XorBlocks(block, out + i, out + i, blockBytesLen);
    memcpy(block, in + i, blockBytesLen);
  }

  delete[] roundKeys;

  return out;
}

unsigned char *AES::EncryptCFB(const unsigned char in[], unsigned int inLen,
                               const unsigned char key[],
                               const unsigned char *iv) {
  CheckLength(inLen);
  unsigned char *out = new unsigned char[inLen];
  unsigned char block[blockBytesLen];
  unsigned char encryptedBlock[blockBytesLen];
  unsigned char *roundKeys = new unsigned char[4 * Nb * (Nr + 1)];
  KeyExpansion(key, roundKeys);
  memcpy(block, iv, blockBytesLen);
  for (unsigned int i = 0; i < inLen; i += blockBytesLen) {
    EncryptBlock(block, encryptedBlock, roundKeys);
    XorBlocks(in + i, encryptedBlock, out + i, blockBytesLen);
    memcpy(block, out + i, blockBytesLen);
  }

  delete[] roundKeys;

  return out;
}

unsigned char *AES::DecryptCFB(const unsigned char in[], unsigned int inLen,
                               const unsigned char key[],
                               const unsigned char *iv) {
  CheckLength(inLen);
  unsigned char *out = new unsigned char[inLen];
  unsigned char block[blockBytesLen];
  unsigned char encryptedBlock[blockBytesLen];
  unsigned char *roundKeys = new unsigned char[4 * Nb * (Nr + 1)];
  KeyExpansion(key, roundKeys);
  memcpy(block, iv, blockBytesLen);
  for (unsigned int i = 0; i < inLen; i += blockBytesLen) {
    EncryptBlock(block, encryptedBlock, roundKeys);
    XorBlocks(in + i, encryptedBlock, out + i, blockBytesLen);
    memcpy(block, in + i, blockBytesLen);
  }

  delete[] roundKeys;

  return out;
}

void AES::CheckLength(unsigned int len) {
  if (len % blockBytesLen != 0) {
    throw std::length_error("Plaintext length must be divisible by " +
                            std::to_string(blockBytesLen));
  }
}

void AES::EncryptBlock(const unsigned char in[], unsigned char out[],
                       unsigned char *roundKeys) {
  unsigned char state[4][Nb];
  unsigned int i, j, round;

  for (i = 0; i < 4; i++) {
    for (j = 0; j < Nb; j++) {
      state[i][j] = in[i + 4 * j];
    }
  }

  AddRoundKey(state, roundKeys);

  for (round = 1; round <= Nr - 1; round++) {
    SubBytes(state);
    ShiftRows(state);
    MixColumns(state);
    AddRoundKey(state, roundKeys + round * 4 * Nb);
  }

  SubBytes(state);
  ShiftRows(state);
  AddRoundKey(state, roundKeys + Nr * 4 * Nb);

  for (i = 0; i < 4; i++) {
    for (j = 0; j < Nb; j++) {
      out[i + 4 * j] = state[i][j];
    }
  }
}

void AES::DecryptBlock(const unsigned char in[], unsigned char out[],
                       unsigned char *roundKeys) {
  unsigned char state[4][Nb];
  unsigned int i, j, round;

  for (i = 0; i < 4; i++) {
    for (j = 0; j < Nb; j++) {
      state[i][j] = in[i + 4 * j];
    }
  }

  AddRoundKey(state, roundKeys + Nr * 4 * Nb);

  for (round = Nr - 1; round >= 1; round--) {
    InvSubBytes(state);
    InvShiftRows(state);
    AddRoundKey(state, roundKeys + round * 4 * Nb);
    InvMixColumns(state);
  }

  InvSubBytes(state);
  InvShiftRows(state);
  AddRoundKey(state, roundKeys);

  for (i = 0; i < 4; i++) {
    for (j = 0; j < Nb; j++) {
      out[i + 4 * j] = state[i][j];
    }
  }
}

void AES::SubBytes(unsigned char state[4][Nb]) {
  unsigned int i, j;
  unsigned char t;
  for (i = 0; i < 4; i++) {
    for (j = 0; j < Nb; j++) {
      t = state[i][j];
      state[i][j] = sbox[t / 16][t % 16];
    }
  }
}

void AES::ShiftRow(unsigned char state[4][Nb], unsigned int i,
                   unsigned int n)  // shift row i on n positions
{
  unsigned char tmp[Nb];
  for (unsigned int j = 0; j < Nb; j++) {
    tmp[j] = state[i][(j + n) % Nb];
  }
  memcpy(state[i], tmp, Nb * sizeof(unsigned char));
}

void AES::ShiftRows(unsigned char state[4][Nb]) {
  ShiftRow(state, 1, 1);
  ShiftRow(state, 2, 2);
  ShiftRow(state, 3, 3);
}

unsigned char AES::xtime(unsigned char b)  // multiply on x
{
  return (b << 1) ^ (((b >> 7) & 1) * 0x1b);
}

void AES::MixColumns(unsigned char state[4][Nb]) {
  unsigned char temp_state[4][Nb];

  for (size_t i = 0; i < 4; ++i) {
    memset(temp_state[i], 0, 4);
  }

  for (size_t i = 0; i < 4; ++i) {
    for (size_t k = 0; k < 4; ++k) {
      for (size_t j = 0; j < 4; ++j) {
        if (CMDS[i][k] == 1)
          temp_state[i][j] ^= state[k][j];
        else
          temp_state[i][j] ^= GF_MUL_TABLE[CMDS[i][k]][state[k][j]];
      }
    }
  }

  for (size_t i = 0; i < 4; ++i) {
    memcpy(state[i], temp_state[i], 4);
  }
}

void AES::AddRoundKey(unsigned char state[4][Nb], unsigned char *key) {
  unsigned int i, j;
  for (i = 0; i < 4; i++) {
    for (j = 0; j < Nb; j++) {
      state[i][j] = state[i][j] ^ key[i + 4 * j];
    }
  }
}

void AES::SubWord(unsigned char *a) {
  int i;
  for (i = 0; i < 4; i++) {
    a[i] = sbox[a[i] / 16][a[i] % 16];
  }
}

void AES::RotWord(unsigned char *a) {
  unsigned char c = a[0];
  a[0] = a[1];
  a[1] = a[2];
  a[2] = a[3];
  a[3] = c;
}

void AES::XorWords(unsigned char *a, unsigned char *b, unsigned char *c) {
  int i;
  for (i = 0; i < 4; i++) {
    c[i] = a[i] ^ b[i];
  }
}

void AES::Rcon(unsigned char *a, unsigned int n) {
  unsigned int i;
  unsigned char c = 1;
  for (i = 0; i < n - 1; i++) {
    c = xtime(c);
  }

  a[0] = c;
  a[1] = a[2] = a[3] = 0;
}

void AES::KeyExpansion(const unsigned char key[], unsigned char w[]) {
  unsigned char temp[4];
  unsigned char rcon[4];

  unsigned int i = 0;
  while (i < 4 * Nk) {
    w[i] = key[i];
    i++;
  }

  i = 4 * Nk;
  while (i < 4 * Nb * (Nr + 1)) {
    temp[0] = w[i - 4 + 0];
    temp[1] = w[i - 4 + 1];
    temp[2] = w[i - 4 + 2];
    temp[3] = w[i - 4 + 3];

    if (i / 4 % Nk == 0) {
      RotWord(temp);
      SubWord(temp);
      Rcon(rcon, i / (Nk * 4));
      XorWords(temp, rcon, temp);
    } else if (Nk > 6 && i / 4 % Nk == 4) {
      SubWord(temp);
    }

    w[i + 0] = w[i - 4 * Nk] ^ temp[0];
    w[i + 1] = w[i + 1 - 4 * Nk] ^ temp[1];
    w[i + 2] = w[i + 2 - 4 * Nk] ^ temp[2];
    w[i + 3] = w[i + 3 - 4 * Nk] ^ temp[3];
    i += 4;
  }
}

void AES::InvSubBytes(unsigned char state[4][Nb]) {
  unsigned int i, j;
  unsigned char t;
  for (i = 0; i < 4; i++) {
    for (j = 0; j < Nb; j++) {
      t = state[i][j];
      state[i][j] = inv_sbox[t / 16][t % 16];
    }
  }
}

void AES::InvMixColumns(unsigned char state[4][Nb]) {
  unsigned char temp_state[4][Nb];

  for (size_t i = 0; i < 4; ++i) {
    memset(temp_state[i], 0, 4);
  }

  for (size_t i = 0; i < 4; ++i) {
    for (size_t k = 0; k < 4; ++k) {
      for (size_t j = 0; j < 4; ++j) {
        temp_state[i][j] ^= GF_MUL_TABLE[INV_CMDS[i][k]][state[k][j]];
      }
    }
  }

  for (size_t i = 0; i < 4; ++i) {
    memcpy(state[i], temp_state[i], 4);
  }
}

void AES::InvShiftRows(unsigned char state[4][Nb]) {
  ShiftRow(state, 1, Nb - 1);
  ShiftRow(state, 2, Nb - 2);
  ShiftRow(state, 3, Nb - 3);
}

void AES::XorBlocks(const unsigned char *a, const unsigned char *b,
                    unsigned char *c, unsigned int len) {
  for (unsigned int i = 0; i < len; i++) {
    c[i] = a[i] ^ b[i];
  }
}

void AES::printHexArray(unsigned char a[], unsigned int n) {
  for (unsigned int i = 0; i < n; i++) {
    printf("%02x ", a[i]);
  }
}

void AES::printHexVector(std::vector<unsigned char> a) {
  for (unsigned int i = 0; i < a.size(); i++) {
    printf("%02x ", a[i]);
  }
}

std::vector<unsigned char> AES::ArrayToVector(unsigned char *a,
                                              unsigned int len) {
  std::vector<unsigned char> v(a, a + len * sizeof(unsigned char));
  return v;
}

unsigned char *AES::VectorToArray(std::vector<unsigned char> &a) {
  return a.data();
}

std::vector<unsigned char> AES::EncryptECB(std::vector<unsigned char> in,
                                           std::vector<unsigned char> key) {
  unsigned char *out = EncryptECB(VectorToArray(in), (unsigned int)in.size(),
                                  VectorToArray(key));
  std::vector<unsigned char> v = ArrayToVector(out, in.size());
  delete[] out;
  return v;
}

std::vector<unsigned char> AES::DecryptECB(std::vector<unsigned char> in,
                                           std::vector<unsigned char> key) {
  unsigned char *out = DecryptECB(VectorToArray(in), (unsigned int)in.size(),
                                  VectorToArray(key));
  std::vector<unsigned char> v = ArrayToVector(out, (unsigned int)in.size());
  delete[] out;
  return v;
}

std::vector<unsigned char> AES::EncryptCBC(std::vector<unsigned char> in,
                                           std::vector<unsigned char> key,
                                           std::vector<unsigned char> iv) {
  unsigned char *out = EncryptCBC(VectorToArray(in), (unsigned int)in.size(),
                                  VectorToArray(key), VectorToArray(iv));
  std::vector<unsigned char> v = ArrayToVector(out, in.size());
  delete[] out;
  return v;
}

std::vector<unsigned char> AES::DecryptCBC(std::vector<unsigned char> in,
                                           std::vector<unsigned char> key,
                                           std::vector<unsigned char> iv) {
  unsigned char *out = DecryptCBC(VectorToArray(in), (unsigned int)in.size(),
                                  VectorToArray(key), VectorToArray(iv));
  std::vector<unsigned char> v = ArrayToVector(out, (unsigned int)in.size());
  delete[] out;
  return v;
}

std::vector<unsigned char> AES::EncryptCFB(std::vector<unsigned char> in,
                                           std::vector<unsigned char> key,
                                           std::vector<unsigned char> iv) {
  unsigned char *out = EncryptCFB(VectorToArray(in), (unsigned int)in.size(),
                                  VectorToArray(key), VectorToArray(iv));
  std::vector<unsigned char> v = ArrayToVector(out, in.size());
  delete[] out;
  return v;
}

std::vector<unsigned char> AES::DecryptCFB(std::vector<unsigned char> in,
                                           std::vector<unsigned char> key,
                                           std::vector<unsigned char> iv) {
  unsigned char *out = DecryptCFB(VectorToArray(in), (unsigned int)in.size(),
                                  VectorToArray(key), VectorToArray(iv));
  std::vector<unsigned char> v = ArrayToVector(out, (unsigned int)in.size());
  delete[] out;
  return v;
}*/