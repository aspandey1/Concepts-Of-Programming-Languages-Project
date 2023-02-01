//Group Members: Kanhai Shah, Ashish Pandey, and Christian Pantoja

#include <iostream>
#include <fstream>
#include <sstream>
#include <ostream>
#include <stack>
using namespace std;

//function declaration
void parser (string program);
string openFile (string fileName);
void scan(string *program, string *tokenWord, string *token);
int getNextState(int currentState, char currentChar);
string finalStateToToken(int currentState);

//global variables
stack <string> parserStack;
int parserTab = 0;
bool parserErrorFlag = false;
string parserOutput;

/*
Input: Takes program string from user
Output: Parser Tree 
Description: Follow the grammer and take tokens from scan function and populate the parser tree
*/
void parser (string program) {
  string tokenWord, token;
  if (parserErrorFlag == false) {
    // if end of the program
    if (parserStack.top() == "</program>") {
      parserOutput.append(parserStack.top() + "\n");
      parserStack.pop();
    }
    else {
      //grammer for the <program>
      if (parserStack.top() == "<program>") {
        parserOutput.append(parserStack.top() + "\n");
        parserStack.pop();
        parserStack.push("</stmt_list>");
        parserStack.push("<stmt_list>");
        parserTab += 1;
      }
      //grammer for the <stmt_list>
      else if (parserStack.top() == "<stmt_list>") {
        if (program[0] == '$') {
          for (int i=0; i<parserTab; i++) {
            parserOutput.append("\t");
          }
          parserTab += 1;
          parserOutput.append(parserStack.top() + "\n");
          parserStack.pop();
        }
        else {
          for (int i=0; i<parserTab; i++) {
            parserOutput.append("\t");
          }
          parserTab += 1;
          parserOutput.append(parserStack.top() + "\n");
          parserStack.pop();
          parserStack.push("</stmt_list>");
          parserStack.push("<stmt_list>");
          parserStack.push("</stmt>");
          parserStack.push("<stmt>");
        }
      }
      //grammer for the <stmt>
      else if (parserStack.top() == "<stmt>") {
        for (int i=0; i<parserTab; i++) {
          parserOutput.append("\t");
        }
        parserTab += 1;
        parserOutput.append(parserStack.top() + "\n");
        parserStack.pop();
        //==============================================
        scan(&program, &tokenWord, &token);
        if (token == "id") {
          for (int i=0; i<parserTab; i++) {
            parserOutput.append("\t");
          }
          parserTab += 1;
          parserOutput.append("<id>\n");
          for (int i=0; i<parserTab; i++) {
            parserOutput.append("\t");
          }
          parserTab += 1;
          parserOutput.append(tokenWord + "\n");
          parserTab -= 2;
          for (int i=0; i<parserTab; i++) {
            parserOutput.append("\t");
          }
          parserOutput.append("</id>\n");
          //====================================
          scan(&program, &tokenWord, &token);
          if (token == "assign") {
            for (int i=0; i<parserTab; i++) {
              parserOutput.append("\t");
            }
            parserTab += 1;
            parserOutput.append("<assign>\n");
            for (int i=0; i<parserTab; i++) {
              parserOutput.append("\t");
            }
            parserTab += 1;
            parserOutput.append(tokenWord + "\n");
            parserTab -= 2;
            for (int i=0; i<parserTab; i++) {
              parserOutput.append("\t");
            }
            parserOutput.append("</assign>\n");
            parserStack.push("</expr>");
            parserStack.push("<expr>");
          }
          else {
            parserErrorFlag = true;
          }
        }
        else if (token == "read") {
          for (int i=0; i<parserTab; i++) {
            parserOutput.append("\t");
          }
          parserTab += 1;
          parserOutput.append("<read>\n");
          for (int i=0; i<parserTab; i++) {
            parserOutput.append("\t");
          }
          parserTab += 1;
          parserOutput.append(tokenWord + "\n");
          parserTab -= 2;
          for (int i=0; i<parserTab; i++) {
            parserOutput.append("\t");
          }
          parserOutput.append("</read>\n");
          //====================================
          scan(&program, &tokenWord, &token);
          if (token == "id") {
            for (int i=0; i<parserTab; i++) {
              parserOutput.append("\t");
            }
            parserTab += 1;
            parserOutput.append("<id>\n");
            for (int i=0; i<parserTab; i++) {
              parserOutput.append("\t");
            }
            parserTab += 1;
            parserOutput.append(tokenWord + "\n");
            parserTab -= 2;
            for (int i=0; i<parserTab; i++) {
              parserOutput.append("\t");
            }
            parserOutput.append("</id>\n");
          }
          else {
            parserErrorFlag = true;
          }
        }
        else if (token == "write") {
          for (int i=0; i<parserTab; i++) {
            parserOutput.append("\t");
          }
          parserTab += 1;
          parserOutput.append("<write>\n");
          for (int i=0; i<parserTab; i++) {
            parserOutput.append("\t");
          }
          parserTab += 1;
          parserOutput.append(tokenWord + "\n");
          parserTab -= 2;
          for (int i=0; i<parserTab; i++) {
            parserOutput.append("\t");
          }
          parserOutput.append("</write>\n");
          parserStack.push("</expr>");
          parserStack.push("<expr>");
        }
        else {
          parserErrorFlag = true;
        }
      }
      //grammer for the <expr>
      else if (parserStack.top() == "<expr>") {
        for (int i=0; i<parserTab; i++) {
          parserOutput.append("\t");
        }
        parserTab += 1;
        parserOutput.append(parserStack.top() + "\n");
        parserStack.pop();
        parserStack.push("</term_tail>");
        parserStack.push("<term_tail>");
        parserStack.push("</term>");
        parserStack.push("<term>");
      }
      //grammer for the <term_tail>
      else if (parserStack.top() == "<term_tail>") {
        if (program[0] != '$') {
          string tmp_program = program;
          scan(&program, &tokenWord, &token);
          if (token == "plus" || token == "minus") { 
            for (int i=0; i<parserTab; i++) {
              parserOutput.append("\t");
            }
            parserTab += 1;
            parserOutput.append(parserStack.top() + "\n");
            parserStack.pop();
            parserStack.push("</term_tail>");
            parserStack.push("<term_tail>");
            parserStack.push("</term>");
            parserStack.push("<term>");
            parserStack.push("</add_op>");
            parserStack.push("<add_op>");
          }
          else {
            for (int i=0; i<parserTab; i++) {
              parserOutput.append("\t");
            }
            parserTab += 1;
            parserOutput.append(parserStack.top() + "\n");
            parserStack.pop();
          }
          program = tmp_program;
        }
        else {
          for (int i=0; i<parserTab; i++) {
            parserOutput.append("\t");
          }
          parserTab += 1;
          parserOutput.append(parserStack.top() + "\n");
          parserStack.pop();
        }
      }
      //grammer for the <term>
      else if (parserStack.top() == "<term>") {
        for (int i=0; i<parserTab; i++) {
          parserOutput.append("\t");
        }
        parserTab += 1;
        parserOutput.append(parserStack.top() + "\n");
        parserStack.pop();
        parserStack.push("</fact_tail>");
        parserStack.push("<fact_tail>");
        parserStack.push("</factor>");
        parserStack.push("<factor>");
      }
      //grammer for the <fact_tail>
      else if (parserStack.top() == "<fact_tail>") {
        if (program[0] != '$') {
          string tmp_program = program;
          scan(&program, &tokenWord, &token);
          if (token == "times" || token == "div") {
            for (int i=0; i<parserTab; i++) {
              parserOutput.append("\t");
            }
            parserTab += 1;
            parserOutput.append(parserStack.top() + "\n");
            parserStack.pop();
            parserStack.push("</fact_tail>");
            parserStack.push("<fact_tail>");
            parserStack.push("</factor>");
            parserStack.push("<factor>");
            parserStack.push("</mult_op>");
            parserStack.push("<mult_op>");
          }
          else {
            for (int i=0; i<parserTab; i++) {
              parserOutput.append("\t");
            }
            parserTab += 1;
            parserOutput.append(parserStack.top() + "\n");
            parserStack.pop();
          }
          program = tmp_program;
        }
        else {
          for (int i=0; i<parserTab; i++) {
            parserOutput.append("\t");
          }
          parserTab += 1;
          parserOutput.append(parserStack.top() + "\n");
          parserStack.pop();
        }
      }
      //grammer for the <factor>
      else if (parserStack.top() == "<factor>") {
        for (int i=0; i<parserTab; i++) {
          parserOutput.append("\t");
        }
        parserTab += 1;
        parserOutput.append(parserStack.top() + "\n");
        parserStack.pop();
        //==============================================
        scan(&program, &tokenWord, &token);
        if (token == "lparen") {
          for (int i=0; i<parserTab; i++) {
            parserOutput.append("\t");
          }
          parserTab += 1;
          parserOutput.append("<lparen>\n");
          for (int i=0; i<parserTab; i++) {
            parserOutput.append("\t");
          }
          parserTab += 1;
          parserOutput.append(tokenWord + "\n");
          parserTab -= 2;
          for (int i=0; i<parserTab; i++) {
            parserOutput.append("\t");
          }
          parserOutput.append("</lparen>\n");
          parserStack.push("</expr>");
          parserStack.push("<expr>");
          //====================================
        }
        else if (token == "id") {
          for (int i=0; i<parserTab; i++) {
            parserOutput.append("\t");
          }
          parserTab += 1;
          parserOutput.append("<id>\n");
          for (int i=0; i<parserTab; i++) {
            parserOutput.append("\t");
          }
          parserTab += 1;
          parserOutput.append(tokenWord + "\n");
          parserTab -= 2;
          for (int i=0; i<parserTab; i++) {
            parserOutput.append("\t");
          }
          parserOutput.append("</id>\n");
        }
        else if (token == "number") {
          for (int i=0; i<parserTab; i++) {
            parserOutput.append("\t");
          }
          parserTab += 1;
          parserOutput.append("<number>\n");
          for (int i=0; i<parserTab; i++) {
            parserOutput.append("\t");
          }
          parserTab += 1;
          parserOutput.append(tokenWord + "\n");
          parserTab -= 2;
          for (int i=0; i<parserTab; i++) {
            parserOutput.append("\t");
          }
          parserOutput.append("</number>\n");
        }
        else {
          parserErrorFlag = true;
        }
      }
      //grammer for the <add_op>
      else if (parserStack.top() == "<add_op>") {
        for (int i=0; i<parserTab; i++) {
          parserOutput.append("\t");
        }
        parserTab += 1;
        parserOutput.append(parserStack.top() + "\n");
        parserStack.pop();
        //==============================================
        scan(&program, &tokenWord, &token);
        if (token == "plus") {
          for (int i=0; i<parserTab; i++) {
            parserOutput.append("\t");
          }
          parserTab += 1;
          parserOutput.append("<plus>\n");
          for (int i=0; i<parserTab; i++) {
            parserOutput.append("\t");
          }
          parserTab += 1;
          parserOutput.append(tokenWord + "\n");
          parserTab -= 2;
          for (int i=0; i<parserTab; i++) {
            parserOutput.append("\t");
          }
          parserOutput.append("</plus>\n");
        }
        else if (token == "minus") {
          for (int i=0; i<parserTab; i++) {
            parserOutput.append("\t");
          }
          parserTab += 1;
          parserOutput.append("<minus>\n");
          for (int i=0; i<parserTab; i++) {
            parserOutput.append("\t");
          }
          parserTab += 1;
          parserOutput.append(tokenWord + "\n");
          parserTab -= 2;
          for (int i=0; i<parserTab; i++) {
            parserOutput.append("\t");
          }
          parserOutput.append("</minus>\n");
        }
        else {
          parserErrorFlag = true;
        }
      }
      //grammer for the <mult_op>
      else if (parserStack.top() == "<mult_op>") {
        for (int i=0; i<parserTab; i++) {
          parserOutput.append("\t");
        }
        parserTab += 1;
        parserOutput.append(parserStack.top() + "\n");
        parserStack.pop();
        //==============================================
        scan(&program, &tokenWord, &token);
        if (token == "times") {
          for (int i=0; i<parserTab; i++) {
            parserOutput.append("\t");
          }
          parserTab += 1;
          parserOutput.append("<times>\n");
          for (int i=0; i<parserTab; i++) {
            parserOutput.append("\t");
          }
          parserTab += 1;
          parserOutput.append(tokenWord + "\n");
          parserTab -= 2;
          for (int i=0; i<parserTab; i++) {
            parserOutput.append("\t");
          }
          parserOutput.append("</times>\n");
        }
        else if (token == "div") {
          for (int i=0; i<parserTab; i++) {
            parserOutput.append("\t");
          }
          parserTab += 1;
          parserOutput.append("<div>\n");
          for (int i=0; i<parserTab; i++) {
            parserOutput.append("\t");
          }
          parserTab += 1;
          parserOutput.append(tokenWord + "\n");
          parserTab -= 2;
          for (int i=0; i<parserTab; i++) {
            parserOutput.append("\t");
          }
          parserOutput.append("</div>\n");
        }
        else {
          parserErrorFlag = true;
        }
      }
      //grammer for the </expr> for the rparen
      else if (parserStack.top() == "</expr>") {
        if (program[0] != '$') {
          string tmp_program = program;
          scan(&program, &tokenWord, &token);
          if (token == "rparen") {
            //==================================
            for (int i=0; i<parserTab; i++) {
              parserOutput.append("\t");
            }
            parserTab += 1;
            parserOutput.append("<rparen>\n");
            for (int i=0; i<parserTab; i++) {
              parserOutput.append("\t");
            }
            parserTab += 1;
            parserOutput.append(tokenWord + "\n");
            parserTab -= 2;
            for (int i=0; i<parserTab; i++) {
              parserOutput.append("\t");
            }
            parserOutput.append("</rparen>\n");
          }
          else {
            program = tmp_program;
            parserTab -= 1;
            for (int i=0; i<parserTab; i++) {
              parserOutput.append("\t");
            }
            parserOutput.append(parserStack.top() + "\n");
            parserStack.pop();
          }
        }
        else {
          parserTab -= 1;
          for (int i=0; i<parserTab; i++) {
            parserOutput.append("\t");
          }
          parserOutput.append(parserStack.top() + "\n");
          parserStack.pop();
        }
      }
      //closing statements
      else {
        parserTab -= 1;
        for (int i=0; i<parserTab; i++) {
          parserOutput.append("\t");
        }
        parserOutput.append(parserStack.top() + "\n");
        parserStack.pop();
      }
      //call parser again
      parser(program);
    }
  }
}

/*
Input: Takes file name from user
Output: Print parser tree to the screen
Description: Asks the user for a file name. Calls the parser function to create a parser tree
*/
int main() {
  // get user file name
  cout << "\n";
  string fileName;
  cout << "parser ";
  cin >> fileName;
  cout << "\n";
  //=========================
  string program;
  program = openFile(fileName);
  if (program != "") {
    program = program.append("$$");
    parserStack.push("</program>");
    parserStack.push("<program>");
    //call parser
    parser (program);
    if (parserErrorFlag == false) {
      cout << parserOutput;
    }
    else {
      cout << "Error." << "\n";
    }
  }
}

/*
Input: File name
Output: Program String
Description: Take user file and open the file and save the file in a program string
*/
string openFile (string fileName) {
  // variables
  string programString;
  programString = "";
  stringstream buffer;
  // read user file and save the file on a string
  buffer << ifstream (fileName).rdbuf();
  programString = buffer.str();
  if (programString == "") {
    cout << "ERROR: File Is Empty / Could Not Be Found";
  }
  return (programString);
}

/*
Input: Program string
Output: First token it finds, the word that was used for that token, and updated program string
Description: This function scans for token that can be created from the program string
*/
void scan (string *program, string *tokenWord, string *token) {
  string runString;
  string programString = *program;
  // variables
  int currentState = 1;
  int programStringIndex = 0;
  // final token array
  string finalTokenArray[programString.length()];
  int finalTokenArrayIndex = 0;
  int finalTokenArrayCounter = 0;
  // raise error flag to skip the print of the array
  bool errorFlag = false;
  // raise special token if read or write is found
  bool specialToken = false;
  // run while loop to the end of the file string
  while(programStringIndex <= programString.length()-1) {
    // if read or write is found
    if (
      (programString[programStringIndex] == 'r' 
      && programString[programStringIndex+1] == 'e' 
      && programString[programStringIndex+2] == 'a' 
      && programString[programStringIndex+3] == 'd') 
      || 
      (programString[programStringIndex] == 'w' 
      && programString[programStringIndex+1] == 'r' 
      && programString[programStringIndex+2] == 'i' 
      && programString[programStringIndex+3] == 't'
      && programString[programStringIndex+4] == 'e')) {
        // if in comment states
        if (currentState == 4 || currentState == 3 || currentState == 5) {
          if (programString[programStringIndex] == 'r') {
            if (programString[programStringIndex+1] == 'e') {
              if (programString[programStringIndex+2] == 'a') {
                if (programString[programStringIndex+3] == 'd') {
                  if (programStringIndex+3 == programString.length()-1) {
                    errorFlag = true;
                    cout << "\n" << "error.1";
                    break;
                  }
                  programStringIndex += 4;
                }
              }
            }
          }
          if (programString[programStringIndex] == 'w') {
            if (programString[programStringIndex+1] == 'r') {
              if (programString[programStringIndex+2] == 'i') {
                if (programString[programStringIndex+3] == 't') {
                  if (programString[programStringIndex+4] == 'e') {
                    if (programStringIndex+4 == programString.length()-1) {
                      errorFlag = true;
                      cout << "\n" << "error.2";
                      break;
                    }
                    programStringIndex += 5;
                  }
                }
              }
            }
          }
        }
        // if not in comment states
        else {
          if (programString[programStringIndex] == 'r') {
            if (programString[programStringIndex+1] == 'e') {
              if (programString[programStringIndex+2] == 'a') {
                if (programString[programStringIndex+3] == 'd') {
                  if(currentState == 1) {
                    finalTokenArray[finalTokenArrayIndex] = "read";
                    finalTokenArrayIndex++;
                    finalTokenArrayCounter++;
                    runString = "read";
                    break;
                  }
                  else {
                    finalTokenArray[finalTokenArrayIndex] = finalStateToToken(currentState);
                    finalTokenArrayIndex++;
                    finalTokenArrayCounter++;
                    currentState = 1;
                    finalTokenArray[finalTokenArrayIndex] = "read";
                    finalTokenArrayIndex++;
                    finalTokenArrayCounter++;
                    currentState = 1;
                    break;
                  }
                  if (programStringIndex+3 == programString.length()-1) {
                    break;
                  }
                  programStringIndex += 4;
                }
              }
            }
          }
          if (programString[programStringIndex] == 'w') {
            if (programString[programStringIndex+1] == 'r') {
              if (programString[programStringIndex+2] == 'i') {
                if (programString[programStringIndex+3] == 't') {
                  if (programString[programStringIndex+4] == 'e') {
                    if(currentState == 1) {
                      finalTokenArray[finalTokenArrayIndex] = "write";
                      finalTokenArrayIndex++;
                      finalTokenArrayCounter++;
                      runString = "write";
                      break;
                    }
                    else {
                      finalTokenArray[finalTokenArrayIndex] = finalStateToToken(currentState);
                      finalTokenArrayIndex++;
                      finalTokenArrayCounter++;
                      currentState = 1;
                      finalTokenArray[finalTokenArrayIndex] = "write";
                      finalTokenArrayIndex++;
                      finalTokenArrayCounter++;
                      currentState = 1;
                      break;
                    }
                    if (programStringIndex+4 == programString.length()-1) {
                      break;
                    }
                    programStringIndex += 5;
                  }
                }
              }
            }
          }
        }
      }
    // else if normal letters are found
    else {
      // get next state with current state and current letter
      int nextState = getNextState(currentState, programString[programStringIndex]);
      // if the next state is not possible given the current character
      if (nextState == -1) {
        // if the current state is the final state
        if (currentState == 2 || currentState == 6 || currentState == 7 || 
            currentState == 8 || currentState == 9 || currentState == 10 || 
            currentState == 12 || currentState == 14 || currentState == 15 || 
            currentState == 16) {
          // add token to the token array
          finalTokenArray[finalTokenArrayIndex] = finalStateToToken(currentState);
          finalTokenArrayIndex++;
          finalTokenArrayCounter++;
          currentState = 1;
          break;
        }
        // else raise error
        else {
          errorFlag = true;
          cout << "\n" << "error.3";
          break;
        }
      }
      // if next state is possible given the current character
      else {
        currentState = nextState;
        // if last letter of the string does not lead to final state
        if (programStringIndex == programString.length()-1) {
          runString += programString[programStringIndex];
          if (currentState == 3 || currentState == 4 || currentState == 5 || currentState == 11 || currentState == 13) {
            errorFlag = true;
            cout << "\n" << "error.4";
            break;
          }
          if (currentState == 1) {
            break;
          }
          finalTokenArray[finalTokenArrayIndex] = finalStateToToken(currentState);
          finalTokenArrayCounter++;
          break;
        }
        // move to the next letter of the string
        runString += programString[programStringIndex];
        programStringIndex++;
      }
    }
  }
  // token word
  string y;
  for (int i=0; i<runString.length(); i++) {
    if (runString[i] == '/' && runString[i+1] == '*') {
      i+=2;
      while (runString[i] != '*' && runString[i+1] != '/') {
        i++;
      }
      i+=2;
    }
    else if (runString[i] == '/' && runString[i+1] == '/') {
      i+=2;
      while (runString[i] != '\n') {
        i++;
      }
      i+=1;
    }
    y += runString[i];
  }
  string a;
  if(y[0] == '\n'){
    for (int i=1; i<y.length(); i++){
      a += y[i];
    }
    *tokenWord = a;
  }
  else {
    *tokenWord = y;
  }
  // token
  *token = finalTokenArray[0];
  // updated program string
  string z;
  for (int i=0; i<programString.length(); i++) {
    if (programString[i] == '/' && programString[i+1] == '*') {
      i+=2;
      while (programString[i] != '*' && programString[i+1] != '/') {
        i++;
      }
      i+=2;
    }
    else if (programString[i] == '/' && programString[i+1] == '/') {
      i+=2;
      while (programString[i] != '\n') {
        i++;
      }
      i+=1;
    }
    z += programString[i];
  }
  string x;
  for (int i=0; i<z.length(); i++) {
    if (z[i] != y[i]) {
      x += z[i];
    }
  }
  *program = x;
}

/*
Input: The current state from the DFA and the current character from the file
Output: The next state from the DFA
Description: This function return the value of the next state based on the current state and current character and returns -1 if the next state is not possible
*/
int getNextState(int currentState, char currentChar) {
  if ((currentChar == ' ' || currentChar == '\t' || currentChar == '\n') && currentState == 1) {
    return 1;
  }
  else if ((currentChar == '(') && currentState == 1) {
    return 6;
  }
  else if ((currentChar == ')') && currentState == 1) {
    return 7;
  }
  else if ((currentChar == '+') && currentState == 1) {
    return 8;
  }
  else if ((currentChar == '-') && currentState == 1) {
    return 9;
  }
  else if ((currentChar == '*') && currentState == 1) {
    return 10;
  }
  else if ((currentChar == ':') && currentState == 1) {
    return 11;
  }
  else if ((currentChar == '=') && currentState == 11) {
    return 12;
  }
  else if ((currentChar == '.') && currentState == 1) {
    return 13;
  }
  else if ((isdigit(currentChar)) && currentState == 13) {
    return 15;
  }
  else if ((isdigit(currentChar)) && currentState == 15) {
    return 15;
  }
  else if ((isdigit(currentChar)) && currentState == 1) {
    return 14;
  }
  else if ((isdigit(currentChar)) && currentState == 14) {
    return 14;
  }
  else if ((currentChar == '.') && currentState == 14) {
    return 15;
  }
  else if ((isalpha(currentChar)) && currentState == 1) {
    return 16;
  }
  else if ((isalpha(currentChar)) && currentState == 16) {
    return 16;
  }
  else if ((isdigit(currentChar)) && currentState == 16) {
    return 16;
  }
  else if ((currentChar == '/') && currentState == 1) {
    return 2;
  }
  else if ((currentChar == '/') && currentState == 2) {
    return 3;
  }
  else if ((currentChar != '\n') && currentState == 3) {
    return 3;
  }
  else if ((currentChar == '\n') && currentState == 3) {
    return 1;
  }
  else if ((currentChar == '*') && currentState == 2) {
    return 4;
  }
  else if ((currentChar != '*') && currentState == 4) {
    return 4;
  }
  else if ((currentChar == '*') && currentState == 4) {
    return 5;
  }
  else if ((currentChar != '/' && currentChar != '*')  && currentState == 5) {
    return 4;
  }
  else if ((currentChar == '*') && currentState == 5) {
    return 5;
  }
  else if ((currentChar == '/') && currentState == 5) {
    return 1;
  }
  else {
    return -1;
  }
}

/*
Input: The current state number
Output: The name of the current state
Description: This function return a string, final state token, based on the current state
*/
string finalStateToToken(int currentState) {
  switch(currentState){
    case 2:
      return "div";
      break;
    case 6:
      return "lparen";
      break;
    case 7:
      return "rparen";
      break;
    case 8:
      return "plus";
      break;
    case 9:
      return "minus";
      break;
    case 10:
      return "times";
      break;
    case 12:
      return "assign";
      break;
    case 14:
      return "number";
      break;
    case 15:
      return "number";
      break;
    case 16:
      return "id";
      break;
  }
  return 0;
}