/*
 * Name        : turing.cpp
 * Author      : William "Amos" Confer
 *
 * License     : Copyright (C) 2024 All rights reserved
 */

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <deque>
#include <cstring>
#include <iomanip>
#include <cstdint>

typedef uint16_t Oll10O10Ol1l0O01l;typedef int16_t O0000llO0OO011ll;typedef union
OlO1O1O00O1100 OlO1O1O00O1100;typedef union ll001O0111lllll ll001O0111lllll;
#define O0OOO000O0OOl0O int
typedef struct OOO1l0ll101lO1ll0 OOO1l0ll101lO1ll0;
#define lO0ll1lO01O0001l1 true
union OlO1O1O00O1100{struct{uint16_t OOl011O1l11001O:8;uint16_t O1ll0lOlO1O100ll:8;
}bytes;Oll10O10Ol1l0O01l OlO0OOO1O0lO10O0O;O0000llO0OO011ll Ol0010O1l1101l;};
#define OO1OO00l10l101l false
#define lOlOl0O1l1110ll  0
#define O0l01l010l1O11O0    1
#define llOlO1l11ll00lO0 bool
#define ll110Oll0O1l1l0OO    2
#define OOl00l01l10O0   3
#define lO1lO1O101OOl1   4
#define OOl1OOllO0l1OO001   5
union ll001O0111lllll{struct{uint16_t ll1l0O0O1lOll1l1:13;uint16_t llllOlOl10OOl0:3;
}O01l1O0O101000;struct{uint16_t OO0OOlOO01O0Ol0:8;uint16_t ll1l0O0O1lOll1l1:5;uint16_t
llllOlOl10OOl0:3;}O01O1lOl011Ol0;struct{uint16_t OO0OOlOO01O0Ol0:8;uint16_t
O001O1101O001l1OO:1;uint16_t ll1l0O0O1lOll1l1:3;uint16_t Ol0O0OOOO1OOl1:1;uint16_t
llllOlOl10OOl0:3;}OO0O00lOlllO0l0;struct{uint16_t O0O1001l011l0:11;uint16_t
O11O01O100llO10lO:1;uint16_t O100O1OOOO00O1O0:
1;uint16_t llllOlOl10OOl0:3;}l0O1100l0O000;struct
{uint16_t OO0OOlOO01O0Ol0:8;uint16_t O001O1101O001l1OO:1;uint16_t ll1l0O0O1lOll1l1:
4;uint16_t llllOlOl10OOl0
:3;}l0Olllll1OlO0Oll;struct{int16_t O1Ol1ll0ll11Oll:13;uint16_t llllOlOl10OOl0:3;}
Oll0101lOOl10;struct{uint16_t ll1l0O0O1lOll1l1:12;uint16_t lOl1Ol000lOOll:1;uint16_t
llllOlOl10OOl0:3;}l1l0O10O1OO0llOO1;OlO1O1O00O1100 l10l010O1l0O1l;};
struct OOO1l0ll101lO1ll0{O0OOO000O0OOl0O lO1l0ll1l10ll1l;ll001O0111lllll lO0l01ll0lllO;};
using namespace std;typedef struct O100OllOO00010 O100OllOO00010;struct O100OllOO00010{
char OO0OOlOO01O0Ol0; llOlO1l11ll00lO0 l1100O10O1OO0;};
#define l00OOO0O0l0Ol (1 << 11)
#define l1l01O1ll11010lO0 (l00OOO0O0l0Ol * sizeof(ll001O0111lllll))
O100OllOO00010 l011O00O10001O(O0OOO000O0OOl0O l11O1OO000l00l,deque<O100OllOO00010>
&OOlOO00Ol1011);void Ol1OO111O1100(O0OOO000O0OOl0O& l11O1OO000l00l, deque<O100OllOO00010
>&OOlOO00Ol1011,O100OllOO00010 llOO0110l0010OO);

void print_program_usage();
void parse_options(int argc, char *argv[], ifstream &bin, ifstream &tapes);
bool option_animate = false;
bool option_debug = false;
bool option_verbose = false;

int main(int argc, char *argv[]){
ifstream l0l1Oll0OlO01; ifstream ll011l01lOlOO0OlO; parse_options(argc,
argv,l0l1Oll0OlO01,ll011l01lOlOO0OlO);ll001O0111lllll OOO0l10lO100Ol1O0[l00OOO0O0l0Ol
];memset(OOO0l10lO100Ol1O0,0,l1l01O1ll11010lO0);l0l1Oll0OlO01.read((char *)
OOO0l10lO100Ol1O0,l1l01O1ll11010lO0);l0l1Oll0OlO01.close();string l01O00111O0O01lO;
O0OOO000O0OOl0O O0l101lOl01OO11O=0;O0OOO000O0OOl0O O0llllOO1lOl1ll0=0;while(getline
(ll011l01lOlOO0OlO,l01O00111O0O01lO)){  llOlO1l11ll00lO0 llO0101l110llOO[256] ={
OO1OO00l10l101l};Oll10O10Ol1l0O01l lOO1l000l0O1Ol0O=0;ll001O0111lllll O010O1l011ll11O;
llOlO1l11ll00lO0 ll100l01lOO1l1l=OO1OO00l10l101l;deque<O100OllOO00010> OOlOO00Ol1011;
for(unsigned ll00Ol00011l01l1=0;ll00Ol00011l01l1 < l01O00111O0O01lO.length();
ll00Ol00011l01l1++){if(l01O00111O0O01lO[ll00Ol00011l01l1] != '\r' && l01O00111O0O01lO[
ll00Ol00011l01l1]!='\n'){O100OllOO00010 OO10lOOOl0l1OO={l01O00111O0O01lO[ll00Ol00011l01l1],
OO1OO00l10l101l};OOlOO00Ol1011.push_back(OO10lOOOl0l1OO);}}
O0OOO000O0OOl0O l11O1OO000l00l=0;O0OOO000O0OOl0O lOO01O1lll11l11lO=0;O0OOO000O0OOl0O
OlOO010lO11l1=0;llOlO1l11ll00lO0 ll10l1O001100l100=OO1OO00l10l101l;llOlO1l11ll00lO0 
l111O10ll0OOl1=OO1OO00l10l101l;llOlO1l11ll00lO0 lOl01000O1ll1=lO0ll1lO01O0001l1;while
(!ll10l1O001100l100){if((lOl01000O1ll1&&option_animate)||option_debug){cout<<"0x"<<
hex<<setw(4)<<setfill('0')<<lOO1l000l0O1Ol0O<<dec<<": ";if(lOl01000O1ll1){O0OOO000O0OOl0O 
lO0OOl000001ll0=(l11O1OO000l00l>=0)?0:-l11O1OO000l00l;for(O0OOO000O0OOl0O ll00Ol00011l01l1
=0;ll00Ol00011l01l1<lO0OOl000001ll0;ll00Ol00011l01l1++)cout<<' ';for(unsigned 
ll00Ol00011l01l1=0;ll00Ol00011l01l1<OOlOO00Ol1011.size();ll00Ol00011l01l1++)cout<<(
OOlOO00Ol1011[ll00Ol00011l01l1].l1100O10O1OO0?' ':OOlOO00Ol1011[
ll00Ol00011l01l1].OO0OOlOO01O0Ol0);cout<<endl;cout<<" ";if(l11O1OO000l00l>=0)for(
O0OOO000O0OOl0O ll00Ol00011l01l1=0;ll00Ol00011l01l1<l11O1OO000l00l;ll00Ol00011l01l1++)
cout<<' ';cout<<'^';}cout<<endl;}lOl01000O1ll1=OO1OO00l10l101l;lOO01O1lll11l11lO++;
O010O1l011ll11O=OOO0l10lO100Ol1O0[lOO1l000l0O1Ol0O];lOO1l000l0O1Ol0O=(lOO1l000l0O1Ol0O+1)
%l00OOO0O0l0Ol;O100OllOO00010 llll0O001l1O00=l011O00O10001O(l11O1OO000l00l,OOlOO00Ol1011);
switch(O010O1l011ll11O.O01l1O0O101000.llllOlOl10OOl0){case lOlOl0O1l1110ll:llO0101l110llOO
[O010O1l011ll11O.O01O1lOl011Ol0.OO0OOlOO01O0Ol0]=lO0ll1lO01O0001l1;break;case 
O0l01l010l1O11O0:if(!llll0O001l1O00.l1100O10O1OO0&&!llO0101l110llOO[(O0OOO000O0OOl0O)
(llll0O001l1O00.OO0OOlOO01O0Ol0)]){l111O10ll0OOl1=OO1OO00l10l101l;ll10l1O001100l100=
lO0ll1lO01O0001l1;break;}if((llll0O001l1O00.l1100O10O1OO0&&
O010O1l011ll11O.OO0O00lOlllO0l0.O001O1101O001l1OO)||(!llll0O001l1O00.l1100O10O1OO0&&
(llll0O001l1O00.OO0OOlOO01O0Ol0==O010O1l011ll11O.OO0O00lOlllO0l0.OO0OOlOO01O0Ol0))){
ll100l01lOO1l1l=lO0ll1lO01O0001l1;}else if(!O010O1l011ll11O.OO0O00lOlllO0l0.Ol0O0OOOO1OOl1
){ll100l01lOO1l1l=false;}break;case ll110Oll0O1l1l0OO:if(
O010O1l011ll11O.l0O1100l0O000.O100O1OOOO00O1O0&&ll100l01lOO1l1l){lOO1l000l0O1Ol0O=
O010O1l011ll11O.l0O1100l0O000.O0O1001l011l0;}else if(
O010O1l011ll11O.l0O1100l0O000.O11O01O100llO10lO&&!ll100l01lOO1l1l){lOO1l000l0O1Ol0O=
O010O1l011ll11O.l0O1100l0O000.O0O1001l011l0;}break;case OOl00l01l10O0:{O100OllOO00010
OO10lOOOl0l1OO={(char)(O010O1l011ll11O.l0Olllll1OlO0Oll.OO0OOlOO01O0Ol0),(llOlO1l11ll00lO0
)O010O1l011ll11O.l0Olllll1OlO0Oll.O001O1101O001l1OO};Ol1OO111O1100(l11O1OO000l00l,
OOlOO00Ol1011,OO10lOOOl0l1OO);}lOl01000O1ll1=lO0ll1lO01O0001l1;break;case 
lO1lO1O101OOl1:l11O1OO000l00l+=O010O1l011ll11O.Oll0101lOOl10.O1Ol1ll0ll11Oll;
OlOO010lO11l1+= abs(O010O1l011ll11O.Oll0101lOOl10.O1Ol1ll0ll11Oll);lOl01000O1ll1=
lO0ll1lO01O0001l1;break;case OOl1OOllO0l1OO001:    ll10l1O001100l100=lO0ll1lO01O0001l1;
l111O10ll0OOl1=O010O1l011ll11O.l1l0O10O1OO0llOO1.lOl1Ol000lOOll;break;default:cerr<<



                "THIS SHOULD NEVER HAPPEN"
				


<< endl;break;}if(option_animate && lOl01000O1ll1){clock_t O0O10ll1l1010ll=clock()+ 
(0.02*CLOCKS_PER_SEC);while(clock()<O0O10ll1l1010ll);}}while(OOlOO00Ol1011.size()
>0&&l11O1OO000l00l>0&&OOlOO00Ol1011[0].l1100O10O1OO0){l11O1OO000l00l--;
OOlOO00Ol1011.pop_front();}cout<<endl<<(l111O10ll0OOl1? "Halted":"Failed");cout<<
" after " <<OlOO010lO11l1<<" moves and "<<lOO01O1lll11l11lO<<" instructions executed"
<<endl;O0OOO000O0OOl0O lO0OOl000001ll0=(l11O1OO000l00l>= 0)?0:-l11O1OO000l00l;for
(O0OOO000O0OOl0O ll00Ol00011l01l1=0;ll00Ol00011l01l1<lO0OOl000001ll0;ll00Ol00011l01l1++
)cout<<' ';for(unsigned ll00Ol00011l01l1=0;ll00Ol00011l01l1<OOlOO00Ol1011.size();
ll00Ol00011l01l1++) cout << (OOlOO00Ol1011[ll00Ol00011l01l1].l1100O10O1OO0 ? ' ' : 
OOlOO00Ol1011[ll00Ol00011l01l1].OO0OOlOO01O0Ol0);cout << endl;if(l11O1OO000l00l >= 0) 
for(O0OOO000O0OOl0O ll00Ol00011l01l1=0;ll00Ol00011l01l1 < l11O1OO000l00l;
ll00Ol00011l01l1++) cout << ' ';cout << '^' << endl;O0llllOO1lOl1ll0 += 
OlOO010lO11l1;O0l101lOl01OO11O += lOO01O1lll11l11lO;} ll011l01lOlOO0OlO.close();cout << 
endl << endl << "Totals across all tapes..." << endl;cout << " moves: " <<
O0llllOO1lOl1ll0 << endl;cout << "instructions: " << O0l101lOl01OO11O << endl << endl;
exit(EXIT_SUCCESS);}O100OllOO00010 l011O00O10001O(O0OOO000O0OOl0O l11O1OO000l00l
,deque<O100OllOO00010>& OOlOO00Ol1011){ O100OllOO00010 O01O011O0l1llO;if(l11O1OO000l00l < 
0||l11O1OO000l00l>=(signed)(OOlOO00Ol1011.size())){O01O011O0l1llO.l1100O10O1OO0=
lO0ll1lO01O0001l1;}else{O01O011O0l1llO=OOlOO00Ol1011[l11O1OO000l00l];}return 
O01O011O0l1llO;}void Ol1OO111O1100(O0OOO000O0OOl0O&l11O1OO000l00l,deque<O100OllOO00010>&
OOlOO00Ol1011,O100OllOO00010 llOO0110l0010OO){O100OllOO00010 lOO1O11010OOOO0;
lOO1O11010OOOO0.OO0OOlOO01O0Ol0=' ';lOO1O11010OOOO0.l1100O10O1OO0=lO0ll1lO01O0001l1;if
(l11O1OO000l00l<0){for(O0OOO000O0OOl0O ll00Ol00011l01l1=0;ll00Ol00011l01l1
<-l11O1OO000l00l-1;ll00Ol00011l01l1++){OOlOO00Ol1011.push_front(lOO1O11010OOOO0);}
OOlOO00Ol1011.push_front(llOO0110l0010OO);l11O1OO000l00l=0;}else if(l11O1OO000l00l>=
(signed)(OOlOO00Ol1011.size())){for(O0OOO000O0OOl0O ll00Ol00011l01l1=
(O0OOO000O0OOl0O)OOlOO00Ol1011.size();ll00Ol00011l01l1<l11O1OO000l00l;ll00Ol00011l01l1++
){OOlOO00Ol1011.push_back(lOO1O11010OOOO0);}OOlOO00Ol1011.push_back(llOO0110l0010OO);
l11O1OO000l00l=(O0OOO000O0OOl0O)OOlOO00Ol1011.size()-1;} else{OOlOO00Ol1011[
l11O1OO000l00l]=llOO0110l0010OO;}return;}

/*
 * print_program_usage()
 *
 * Prints turing usage and command argument details.
 */
void print_program_usage() {

	cout << "Usage:  turing [-a|--animate] [-d|--debug] [-h|--help] [-v|--verbose] bin_file tapes_file" << endl;
	cout << "" << endl;
	cout << "Options:" << endl;
	cout << "     -a, --animate    Animate tape content and head changes." << endl;
	cout << "     -d, --debug      Follow code execution by address (compare to listing file generated" << endl;
	cout << "                      by tmasm).  Tape only rendered when head or content changes." << endl;
	cout << "     -h, --help       Print this help information." << endl;
	cout << "     -v, --verbose    Same as -a -d" << endl;
	cout << "" << endl;
	cout << "Example:  turing -a demo.bin demo.tape" << endl;
	cout << "     Executes the Turung Machine program from demo.bin separately on the tapes found in" << endl;
	cout << "     the demo.tape file.  Tape files represent a separate tape for each line in the file." << endl;
	cout << "" << endl;

	return;
}


/*
 * parse_options()
 *
 * Processes command arguments and opens binary and tape files.
 */
void parse_options(int argc, char *argv[], ifstream &bin, ifstream &tapes) {

	int l01Ol0lOl10O0Ol0 = 0;
	int OO1O1lll10lOl1O1 = 0;
	int lO0ll1llOOl1l1l = 0;

	char *optarg;
	int optind = 1;

	while(optind < argc && (optarg = argv[optind]) && optarg[0] == '-') {

		if(strcmp(optarg, "-h") == 0 || strcmp(optarg, "--help") == 0) {
			print_program_usage();
			exit(EXIT_SUCCESS);
		}
		else if(strcmp(optarg, "-a") == 0 || strcmp(optarg, "--animate") == 0) {
			option_animate = true;
			l01Ol0lOl10O0Ol0++;
		}
		else if(strcmp(optarg, "-d") == 0 || strcmp(optarg, "--debug") == 0) {
			option_debug = true;
			OO1O1lll10lOl1O1++;
		}
		else if(strcmp(optarg, "-v") == 0 || strcmp(optarg, "--verbose") == 0) {
			option_animate = true;
			option_debug = true;
			option_verbose = true;
			lO0ll1llOOl1l1l++;
		}
		else {
			print_program_usage();
			cerr << "Unknown option : " << optarg << endl;
			exit(1);
			break;
		}

		optind++;
	}

	/*
	 * Check for basic error conditions
	 */

	if(optind != (argc - 2)) {
		print_program_usage();
		exit(1);
	}

	if(l01Ol0lOl10O0Ol0 > 1 || OO1O1lll10lOl1O1 > 1 || lO0ll1llOOl1l1l > 1) {
		cerr << "Not sure what is intended by repeat flag usage..." << endl;
		print_program_usage();
	}

	/*
	 * Open binary and tape files
	 */

	bin.open(argv[optind], ifstream::binary);
	if(!bin.is_open()) {
		cerr << "Could not open file :\"" << argv[optind] << "\"" << endl;
		exit(1);
	}

	optind++;

	tapes.open(argv[optind], ifstream::binary);
	if(!tapes.is_open()) {
		cerr << "Could not open file :\"" << argv[optind] << "\"" << endl;
		exit(1);
	}

	return;
}
