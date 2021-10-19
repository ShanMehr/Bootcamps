/*
Laplace Expansion & Encryption
VP Alexander Le, ACM Los Rios
Algorithms Bootcamp, C++ 
*/

// Preprocessor Directives 
#include <iostream>
#include <vector>
#include <chrono>
#include <ctime>
#include <thread>
#include <unistd.h>

// Namespace Declarations
using namespace std;

// Classes
class Laplace{
  private:

  public:
    /* Default constructor */
    Laplace(){program_start();}
    /* algorithm relevant methods */
    int modInverse(int, int );
    /* get cofactor */
    void getCofactor(vector<vector<int> > &a, vector<vector<int> > &temp, int, int, int);
    /* get determinant */
    int determinant(vector<vector<int> > &a, int, int);
    /* get adjoint */
    void adjoint(vector<vector<int> > &a,vector<vector<int> > &adj,int );
    /* get matrix inverse */
    bool inverse(vector<vector<int> > &a, vector<vector<int> > &inv, int);
    /* visual methods */
    void fast(string, unsigned long, unsigned long);
    void loading();
    void program_start();
    void color(int);
    /* custom deconstructor*/
    ~Laplace(){}
};

int main()
{
    // Encrypt
    Laplace encrypt;
    // Seed rand()
    srand((unsigned)time(NULL));
    int x,y,k,n;
    // Demonstrate encryption with a 2x2 matrix
    n = 2;
    int key[n][n];
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            key[i][j] = rand()%10;
        }
    }

    // Display generated key
    cout << "\nGenerated key:\n" << "[ " << key[0][0] << " " << key[0][1] << " ] "
         << "\n[ " << key[1][0] << " " << key[1][1] << " ] \n"; 

    /* // Optionally allow user to specify key dimensions
    cout<<"Enter the size of key matrix\n";
    cin>>n;
    cout<<"Enter the key matrix\n";
    int key[n][n];
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            cin>>key[i][j];
        }
    }
  */

    encrypt.fast("\nWhat's the message?\n-> ", 15000,0);
    string plaintext;
    cin >> plaintext;
    /* Perfom cofactor elimination on plaintext by the generated nxn dimensions of the key */
    int temp = (n-plaintext.size()%n)%n;
    /* Parse through message string (plaintext) and delimit each character with _ for conversion to the corresponding alphanumerical index 0-27 */
    for(int i=0;i<temp;i++){
        plaintext+='_';
    }
    /* Set our k = 0, will use to process array indexes */
    k=0;
    /* Declare the string to hold the ciphertext */
    string ciphertext="";
    /* While our constant k is less than the size of our plaintext string */
    while(k<plaintext.size()){
      /* Loop through our digraphs */
        for(int i=0;i<n;i++){
            /* Set initial summation at zero */
            int sum = 0;
            int cofactor = k;
            for(int j=0;j<n;j++){
                /* Perform matrix multiplication with our key and parsed digraphs */
                sum += (key[i][j]%27*(plaintext[temp++]-'_')%27)%27;
                sum = sum%27;
            }
            /* Convert back our digraph into characters */
            ciphertext += (sum+'_');
        }
        /* Refresh our constant k for the next pass*/
        k += n; 
        // Reset to n to multiply by key matrix correctly
    } // Pass complete, message is encrypted.
    
    /* Display result with visual flair */
    encrypt.loading();
    cout << "\n" << flush;
    encrypt.fast("\nResult:\n",15000,0);
    cout<<ciphertext<<'\n';

  /*

  // WORK IN PROGRESS
    // Decrypt

    // int x,y,i,j,k,n;
    cout<<"Enter the size of key matrix\n";
    cin>>n;
    cout<<"Enter the key matrix\n";

    vector<vector<int> > a(n, vector<int>(n));
    vector<vector<int> > adj(n, vector<int>(n));
    vector<vector<int> > inv(n, vector<int>(n));

    for(i=0;i<n;i++){
        for(j=0;j<n;j++){
            cin>>key[i][j];
        }
    }
   
    if(inverse(a,inv,n)){
        cout<<"Inverse exist\n";
    }
    cout<<"Enter the message to decrypt\n";
    //string s;
    cin>>s;
    k=0;
    //string ans;
    while(k<s.size()){
        for(i=0;i<n;i++){
            int sum = 0;
            int temp = k;
            for(j=0;j<n;j++){
                sum += ((inv[i][j] + 26)%26*(s[temp++]-'a')%26)%26;
                sum = sum%26;
            }
            ans+=(sum+'a');
        }
        k+=n;
    }
    //ans+='\0';
    int f=ans.size()-1;
    while(ans[f]=='x'){
        f--;
    }
    for(i=0;i<=f;i++){
        cout<<ans[i];
    }
    cout<<'\n';

    */
    return 0;    
}

/* Calculate the matrix key inverse for decryption */
int Laplace::modInverse(int a, int m){ 
    a=a%m; 
    for(int x=-m;x<m;x++) 
       if((a*x)%m==1) 
          return x; 
    return 0;
} 

/* Calculate the matrix key cofactor for decryption */
void Laplace::getCofactor(vector<vector<int> > &key, vector<vector<int> > &temp, int p, int q, int n){ 
    int i=0,j=0; 
    for(int row=0;row<n;row++){ 
        for(int col=0;col<n;col++){ 
            if(row!=p&&col!=q){ 
                temp[i][j++] = key[row][col]; 
                if (j==n-1){ 
                    j=0; 
                    i++; 
                } 
            } 
        } 
    } 
}
/* Calculate the matrix key determinant using Laplace Expansion for decryption */
int Laplace::determinant(vector<vector<int> > &key, int n, int N){ 
    int D = 0;
    if(n==1) 
        return key[0][0]; 
    vector<vector<int> > temp(N, vector<int>(N));
    int sign = 1;
    for(int f=0;f<n;f++){ 
        getCofactor(key, temp, 0, f, n); 
        D += sign * key[0][f] * determinant(temp, n - 1, N); 
        sign = -sign; 
    }
    return D; 
} 
/* Calculate the matrix key adjoint for decryption */
void Laplace::adjoint(vector<vector<int> > &a,vector<vector<int> > &adj,int N){ 
    if(N == 1){ 
        adj[0][0] = 1; 
        return; 
    } 
    int sign = 1;
    vector<vector<int> > temp(N, vector<int>(N));
    for(int i=0;i<N;i++){ 
        for(int j=0;j<N;j++){ 
            getCofactor(a, temp, i, j, N); 
            sign = ((i+j)%2==0)? 1: -1; 
            adj[j][i] = (sign)*(determinant(temp, N-1 , N)); 
        } 
    } 
} 
/* Calculate the matrix key sign map for Laplace Expansion */
bool Laplace::inverse(vector<vector<int> > &a, vector<vector<int> > &inv, int N){ 
    int det = determinant(a, N, N); 
    if(det == 0){ 
        cout << "Inverse does not exist"; 
        return false; 
    } 
    int invDet = modInverse(det,26);
    cout<<det%26<<' '<<invDet<<'\n';
    vector<vector<int> > adj(N, vector<int>(N));
    adjoint(a, adj, N);  
    for(int i=0;i<N;i++) 
        for(int j=0;j<N;j++) 
            inv[i][j] = (adj[i][j]*invDet)%26; 
    return true; 
} 

/* The following functions serve graphical effects only. */

// Program Greeting
void Laplace::program_start(){

  // Title
  cout << "\033[32m"; // Make text green
  fast("Laplace Expansion & Hill Cipher Encryption\n", 15000,0);
  cout<< "\033[m";    // Make text white

}

// Effects 
void Laplace::fast(string text, unsigned long delay, unsigned long postDelay = 1500000)
{
  /* Function scope specific namespace declaration */
  using std::this_thread::sleep_for;
  using std::chrono::milliseconds;

  cout << " " << flush;
  // Loop through each character of the string
  for (unsigned int i = 0; i < text.length(); i++)
  {
    // Print one character as a typed effect
    cout << "\b \b" << text[i] << ' ' << flush;
    usleep(delay);
  }
  // Blink cursor for the post delay
  for (unsigned long post = postDelay; post > 0; post -= 500000)
  {
    cout << "\b " << flush;
    usleep(250000);
    cout << "\b" << flush;
    usleep(250000);
  }
  // Clear cursor
  cout << "\b \b" << flush;
}

void Laplace::loading(){
  // Loading animation
  cout << "\033[32m"; // Make text green
  cout << '-' << flush;
    for (;;) {
        usleep(250000);
        cout << "\b\\" << flush;
        usleep(250000);
        cout << "\b|" << flush;
        usleep(250000);
        cout << "\b/" << flush;
        usleep(250000);
        cout << "\b*" << flush;
        usleep(250000);
        cout << "\b-" << flush;
        break;
    }
    cout << "done\n"<< flush;
    cout<< "\033[m";    // Make text white
}
/* Yet to be used */
void Laplace::color(int n){
  switch(n){
    case 1:
      cout << "\033[32m"; // Make text green
    case 2:
      break;
  }
}



// References
/* 
Polygraphic Substitution Ciphers: The Hill cipher
- https://www.nku.edu/~christensen/1402%20Hill%20cipher.pdf

NSA 
A World War II German Army Field Cipher and
How We Broke It 
- https://www.nsa.gov/portals/75/documents/news-features/declassified-documents/cryptologic-quarterly/world_war_II.pdf

Jim Sanborn - CIA Kryptos Sculpture
-
https://en.wikipedia.org/wiki/Kryptos

Science Olympiad: Codebusters
-
https://scioly.org/wiki/index.php/Codebusters

Historical Precedence for Ciphers
-
https://people-ece.vse.gmu.edu/coursewebpages/ECE/ECE646/F19/viewgraphs_F18/ECE646_lecture6_historical_6.pdf

Additional Slides
-
https://www.docsity.com/en/hill-cipher-computer-security-lecture-slides/79494/
*/

/* Objective:
- Look at Laplace Expansion Theorem
- Explain relationship between matrices and cryptography
- Introduce Digraph Ciphers
- Historical relevancy to warfare 
- Implement a Hill Cipher
- Implement an Afine Cipher
- Part 2: Vignere Cipher
*/


/* ASCII Table for reference
Dec  Char                           Dec  Char     Dec  Char     Dec  Char
---------                           ---------     ---------     ----------
  0  NUL (null)                      32  SPACE     64  @         96  `
  1  SOH (start of heading)          33  !         65  A         97  a
  2  STX (start of text)             34  "         66  B         98  b
  3  ETX (end of text)               35  #         67  C         99  c
  4  EOT (end of transmission)       36  $         68  D        100  d
  5  ENQ (enquiry)                   37  %         69  E        101  e
  6  ACK (acknowledge)               38  &         70  F        102  f
  7  BEL (bell)                      39  '         71  G        103  g
  8  BS  (backspace)                 40  (         72  H        104  h
  9  TAB (horizontal tab)            41  )         73  I        105  i
 10  LF  (NL line feed, new line)    42  *         74  J        106  j
 11  VT  (vertical tab)              43  +         75  K        107  k
 12  FF  (NP form feed, new page)    44  ,         76  L        108  l
 13  CR  (carriage return)           45  -         77  M        109  m
 14  SO  (shift out)                 46  .         78  N        110  n
 15  SI  (shift in)                  47  /         79  O        111  o
 16  DLE (data link escape)          48  0         80  P        112  p
 17  DC1 (device control 1)          49  1         81  Q        113  q
 18  DC2 (device control 2)          50  2         82  R        114  r
 19  DC3 (device control 3)          51  3         83  S        115  s
 20  DC4 (device control 4)          52  4         84  T        116  t
 21  NAK (negative acknowledge)      53  5         85  U        117  u
 22  SYN (synchronous idle)          54  6         86  V        118  v
 23  ETB (end of trans. block)       55  7         87  W        119  w
 24  CAN (cancel)                    56  8         88  X        120  x
 25  EM  (end of medium)             57  9         89  Y        121  y
 26  SUB (substitute)                58  :         90  Z        122  z
 27  ESC (escape)                    59  ;         91  [        123  {
 28  FS  (file separator)            60  <         92  \        124  |
 29  GS  (group separator)           61  =         93  ]        125  }
 30  RS  (record separator)          62  >         94  ^        126  ~
 31  US  (unit separator)            63  ?         95  _        127  DEL
*/
