// XML2RTF.cpp : Defines the entry point for the console application.
//
// ported from QuickBASIC on 4/11/23
//   (Basic version: summer 2023 FUCK wordpress DIE yankees!)

#include "stdafx.h"
#include "XML2RTF.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// The one and only application object

CWinApp theApp;

using namespace std;


void checkBR();
CTimeEx calcdate(CString);
CString getdate();

/* converter for Wordpress Export XML files to RTF, so that the idiots over there won't cause any more damage blocking blogs for no reason
' G.Dar 2/7/2023-26/7/2023
*/

class CStdioFileEx : public CStdioFile {
public:
	void WriteStringLn() { CStdioFile::WriteString("\n"); }
	void WriteStringLn(const char *s) { CStdioFile::WriteString(s); WriteStringLn(); }
	};

// https://www.biblioscape.com/rtf15_spec.htm
// https://www.pcjs.org/documents/books/mspl13/basic/qblang/

//CLS
//DIM SHARED of$
CString iff, off;
CStdioFileEx f1,f2;

struct POST {
	CStringEx title;
	CStringEx date;
	CStringEx author;
	CStringEx content;
	CStringEx link;
	CStringEx categories;
	};
struct COMMENT {
	CStringEx date;
	CStringEx author;
	CStringEx content;
	};

struct POST onepost;
struct COMMENT onecomment;

CTimeEx postdate,oldpostdate;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[]) {
	int nRetCode = FALSE;
	BOOL first;
	BOOL initem=FALSE,indate=FALSE,intitle=FALSE,inlink=FALSE,inimage=FALSE,incomment=FALSE,incomment2=FALSE,incomment3=FALSE;
	BOOL inskip=FALSE,inwp=FALSE,intag=FALSE;
	DWORD totPages=1000,totWords=1000000,totChars=1000000,totCharsws=1000000;

	CStringEx S,a,line;
	char myBuf[2048];
	char myBuf2[65536];

	// initialize MFC and print and error on failure
	if(!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0)) {
		// TODO: change error code to suit your needs
		cerr << _T("Fatal Error: MFC initialization failed") << endl;
		nRetCode = TRUE;
		}
	else 	{
		// TODO: code your application's behavior here.
		CString strHello;
		strHello.LoadString(IDS_HELLO);
		cout << (LPCTSTR)strHello << endl;
		}

	iff = "wp.xml";
	off = "wpout.rtf";
	printf("Conversione file %s\n", iff);

	f1.Open(iff,CFile::modeRead | CFile::shareDenyWrite | CFile::typeText);
	f2.Open(off,CFile::modeCreate | CFile::modeReadWrite | CFile::shareDenyWrite | CFile::typeText);

	DWORD tot = f1.GetLength(),precpos=0,cntDivider=tot/1000;
	CString monthref;

	first = TRUE;
	f2.WriteStringLn("{\\rtf1\\ansi\\ansicpg1252\\uc1 \\deff0\\deflang1033\\deflangfe1040");
//pca850
	f2.WriteStringLn("{\\fonttbl");
	f2.WriteStringLn("{\\f0\\froman\\fcharset0\\fprq2{\\*\\panose 02020603050405020304}Times New Roman;}");
	f2.WriteStringLn("{\\f1\\fswiss\\fcharset0\\fprq2{\\*\\panose 020b0604020202020204}Arial;}");
	//f2.WriteStringLn("{\\f16\froman\\fcharset238\\fprq2 Times New Roman CE;}");
	//f2.WriteStringLn("{\\f17\froman\\fcharset204\\fprq2 Times New Roman Cyr;}");
	//f2.WriteStringLn("{\\f19\froman\\fcharset161\\fprq2 Times New Roman Greek;}");
	//f2.WriteStringLn("{\\f20\froman\\fcharset162\\fprq2 Times New Roman Tur;}");
	//f2.WriteStringLn("{\\f21\froman\\fcharset186\\fprq2 Times New Roman Baltic;}");
	//f2.WriteStringLn("{\\f22\fswiss\\fcharset238\\fprq2 Arial CE;}");
	//f2.WriteStringLn("{\\f23\fswiss\\fcharset204\\fprq2 Arial Cyr;}");
	//f2.WriteStringLn("{\\f25\fswiss\\fcharset161\\fprq2 Arial Greek;}");
	//f2.WriteStringLn("{\\f26\fswiss\\fcharset162\\fprq2 Arial Tur;}");
	//f2.WriteString("{\\f27\\fswiss\\fcharset186\\fprq2 Arial Baltic;}");
	f2.WriteStringLn("}");
	f2.WriteString("{\\colortbl;\\red0\\green0\\blue0;\\red0\\green0\\blue255;\\red0\\green255\\blue255;\\red0\\green255\\blue0;\\red255\\green0\\blue255;\\red255\\green0\\blue0;\\red255\\green255\\blue0;\\red255\\green255\\blue255;\\red0\\green0\\blue128;");
	f2.WriteStringLn("\\red0\\green128\\blue128;\\red0\\green128\\blue0;\\red128\\green0\\blue128;\\red128\\green0\\blue0;\\red128\\green128\\blue0;\\red128\\green128\\blue128;\\red192\\green192\\blue192;}");
	f2.WriteStringLn("{\\stylesheet{\\widctlpar\\adjustright \\fs20\\lang1040\\cgrid \\snext0 Normal;}");
	f2.WriteStringLn("{\\s1\\sb240\\sa60\\keepn\\widctlpar\\adjustright \\b\\f1\\fs28\\lang1040\\kerning28\\cgrid \\sbasedon0 \\snext0 heading 1;}");
	f2.WriteStringLn("{\\*\\cs10 \\additive Default Paragraph Font;}");
	f2.WriteStringLn("{\\*\\cs15 \\additive \\ul\\cf2 \\sbasedon10 Hyperlink;}}");


	sprintf(myBuf,"{\\info{\\title %s converted by G.Dar converter on %s}",(LPCTSTR)iff,(LPCTSTR)CTimeEx::getNowGMT(FALSE));
	f2.WriteString(myBuf);
	f2.WriteString("{\\author Dario Greggio}{\\operator dario Greggio}");
	sprintf(myBuf,"{\\creatim%s}{\\revtim%s}",(LPCTSTR)getdate(),(LPCTSTR)getdate());
	f2.WriteString(myBuf);
	sprintf(myBuf,"{\\version1}{\\edmins1}{\\nofpages%u}{\\nofwords%u}{\\nofchars%u}{\\nofcharsws%u}",
					totPages,totWords,totChars,totCharsws);
	// FINIRE conteggio!
	f2.WriteString(myBuf);
	f2.WriteString("{\\*\\company Dario's Automation which fucks wordpress and automattic of shit}");
	f2.WriteStringLn("{\\vern1}}");

	while(f1.Read(myBuf,1)>0) {
		a = myBuf[0];
 
		switch(myBuf[0]) {
			case '<':
				if(!intag) {
					if(!inwp) {
						if(!inskip && !line.IsEmpty()) {
							if(line.Left(1) != '\n') {
								if(line.Left(2) != "]]") {
									f2.WriteString(line);
	//\\pard\\plain \\s1\\sb240\\sa60\\keepn\\widctlpar\\outlinelevel0\\adjustright \\b\\f1\\fs28\\lang1040\\kerning28\\cgrid {
	//                onepost.content = onepost.content + line$
									}
								else {
									f2.WriteStringLn("\\par ");
									}
							
								if(!inimage && !inlink && !intitle)
									f2.WriteStringLn("\\par ");
								else 
									f2.WriteStringLn();
								if(indate) {
									onepost.date = line;
									postdate = calcdate(line);
									monthref = line.Mid(8,8);
									}
								if(inlink) {
									onepost.link = line;
								}
							}
						}
					else {
						}

					}

				intag = TRUE;
				line.Empty();
				break;

			case '>':
				if(!intag) {
					if(!inwp) {
						if(!inskip && !line.IsEmpty()) {
							if(line.Left(1) != '\n') {
								if(line.Left(2) != "]]") {
									f2.WriteString(line);

	//                onepost.content = onepost.content + line$
									}
								else {
									f2.WriteStringLn("\\par ");
									}
								if(!inimage && !inlink && !intitle)
									f2.WriteStringLn("\\par ");
								else
									f2.WriteStringLn();
								}
							}
						}
					else {
						}

					}		// intag
				else {
					if(line=="p") {			// switch...
						f2.WriteStringLn("{\\qj ");
						}
					else if(line=="/p") {
						f2.WriteStringLn("}");
						}
					else if(line=="title") {
						if(!initem) {
							f2.WriteStringLn("{\\pard\\fs36\\i\\b ");    //f2.WriteStringLn("{\\title ");
	//f2.WriteString("{\\info";"{\\title ";);
							}
						else {
							f2.WriteStringLn("{\\pard\\fs28\\ul ");
							if(!onepost.categories.IsEmpty()  || !onecomment.content.IsEmpty()) {
								f2.WriteStringLn("\\par ");
								}
							}
						intitle = TRUE;
						}
					else if(line=="/title") {
						if(!initem) {
							f2.WriteStringLn("}");
						// FINIRE esce anche con IMG SRC..
							}
						else {
							f2.WriteStringLn("}");
							}

	//           {\\version2}
	//          {\\edmins1}
	//           {\\nofpages2}
	//           {\\nofwords271}
	//           {\\nofchars1547}
	//           {\\\\*\\company oetztal automation}
	//           {\\nofcharsws1899}
	//           {\\vern89}
	//           }\\paperw11906\\paperh16838\\margl1134\\margr1134\\margt1417\\margb1134
	//\\deftab708\\widowctrl\\ftnbj\\aenddoc\\hyphhotz283\\formshade\\viewkind1\\viewscale100\\pgbrdrhead\\pgbrdrfoot \\fet0\\sectd \\linex0\\headery709\\footery709\\colsx709\\endnhere\\sectdefaultcl
	//{\\\\*\\pnseclvl1\\pnucrm\\pnstart1\\pnindent720\\pnhang{\\pntxta .}}
	//{\\\\*\\pnseclvl2\\pnucltr\\pnstart1\\pnindent720\\pnhang{\\pntxta .}}
	//{\\\\*\\pnseclvl3\\pndec\\pnstart1\\pnindent720\\pnhang{\\pntxta .}}
	//{\\\\*\\pnseclvl4\\pnlcltr\\pnstart1\\pnindent720\\pnhang{\\pntxta )}}
	//{\\\\*\\pnseclvl5\\pndec\\pnstart1\\pnindent720\\pnhang{\\pntxtb (}{\\pntxta )}}
	//{\\\\*\\pnseclvl6\\pnlcltr\\pnstart1\\pnindent720\\pnhang{\\pntxtb (}{\\pntxta )}}
	//{\\\\*\\pnseclvl7\\pnlcrm\\pnstart1\\pnindent720\\pnhang{\\pntxtb (}{\\pntxta )}}
	//{\\\\*\\pnseclvl8\\pnlcltr\\pnstart1\\pnindent720\\pnhang{\\pntxtb (}{\\pntxta )}}
	//{\\\\*\\pnseclvl9\\pnlcrm\\pnstart1\\pnindent720\\pnhang{\\pntxtb (}{\\pntxta )}
	//}\\pard\\plain \\s1\\sb240\\sa60\\keepn\\widctlpar\\outlinelevel0\\adjustright \\b\\f1\\fs28\\lang1040\\kerning28\\cgrid {Il riscrittore di esistenze
	//\\par }\\pard\\plain \\widctlpar\\adjustright \\fs20\\lang1040\\cgrid {

						intitle = FALSE;
						}
					else if(line=="image") {    // qua non abbiamo immagini esterne linkate...!
						f2.WriteStringLn("{{\\*\\shppict{\\pict{\\*\\picprop\\shplid1025{\\sp{\\sn shapeType}{\\sv 75}}{\\sp{\\sn fFlipH}{\\sv 0}}{\\sp{\\sn fFlipV}{\\sv 0}}{\\sp{\\sn pibName}");
	//           {\\sv \\\\'5c\\'5c\\'73\\'6b\\'79\\'6e\\'65\\'74\\'5c\\'70\\'72\\'69\\'76\\'61\\'74\\'65\\'5c\\'73\\'61\\'6e\\'74\\'6f\\'67\\'64\\'2e\\'6a\\'70\\'65\\'67}}
	//           {\\sp{\\sn pibFlags}{\\sv 2}}{\\sp{\\sn fillColor}{\\sv 268435473}}{\\sp{\\sn fFilled}{\\sv 0}}{\\sp{\\sn fLine}{\\sv 0}}}
						f2.WriteStringLn("\\picscalex100\\picscaley100\\piccropl0\\piccropr0\\piccropt0\\piccropb0\\picw2646\\pich2646\\picwgoal1500\\pichgoal1500\\jpegblip\\bliptag-12486997");
	//           {\\\\*\\blipuid ff4176ab418208a6a5c4f4f8a13d60d9}ffd8ffe000104a46494600010100000100010000ffe202284943435f50524f46494c450001010000021800000000043000006d6e74725247422058595a200000

						onepost.content += "<img src= ";
						inimage = TRUE;
						}
					else if(line=="/image") {           // finire... e togliere width/height
						f2.WriteStringLn("}}}}}");
						onepost.content += "}}}}}";
						inimage = FALSE;
						}
					else if(line=="item") {
						initem = TRUE;
						}
					else if(line=="/item") {
						initem = FALSE;
						}
					else if(line=="link") {
						f2.WriteString("{\\par\\bkmkstart ");
						onepost.content += "{\\bkmkstart ";
						inlink = TRUE;
						// la GRAFFA andrebbe chiusa DOPO il link...
						}
					else if(line=="/link") {
						f2.WriteStringLn("\\par }{\\bkmkend}");
						onepost.content += "\\par }{\\bkmkend ";
						inlink = FALSE;
						}
					else if(line=="pubDate") {
						f2.WriteStringLn("{\\pard\\fs22\\li0\\b");
						onepost.content += "{\\b";
						indate = TRUE;
						}
					else if(line=="/pubDate") {
						indate = FALSE;
						f2.WriteStringLn("}");
						onepost.content += "}";
						}
					else if(line=="description") {
						f2.WriteStringLn("{\\b\\jc");            //
						onepost.content += "{\\b";
						}
					else if(line=="/description") {
						f2.WriteStringLn("}");
						onepost.content += "}";
						}
							
					else if(line=="wp:comment_content") {
						incomment = TRUE;
						}
					else if(line=="/wp:comment_content") {
						incomment = FALSE;
						}
					else if(line=="wp:comment_date") {
						incomment2 = TRUE;
						}
					else if(line=="/wp:comment_date") {
						incomment2 = FALSE;
						}
					else if(line=="wp:comment_author") {
						incomment3 = TRUE;
						}
					else if(line=="/wp:comment_author") {
						incomment3 = FALSE;
						}

					else if(line=="em") {
						checkBR();
						f2.WriteStringLn("{\\i");
						onepost.content += "{\\i";
						}
					else if(line=="/em") {
						checkBR();
						f2.WriteStringLn("}");
						onepost.content += "}";
						}
					else if(line=="strong") {
						checkBR();
						f2.WriteStringLn("{\\b");
						onepost.content += "{\\b";
						}
					else if(line=="/strong") {
						checkBR();
						f2.WriteStringLn("}");
						onepost.content += "}";
						}
					else if(line=="strike") {
						checkBR();
						f2.WriteStringLn("<strike>");
						onepost.content += "<strike>";
						}
					else if(line=="/strike") {
						checkBR();
						f2.WriteStringLn("</strike>");
						onepost.content += "</strike>";
						}
	//          case "span style=" + CHR$(34) + "text/decoration: underline" + CHR$(34)
					else if(line=="span style=\"text-decoration: underline\"") {
						checkBR();
						f2.WriteStringLn("{\\ul");
						onepost.content += "{\\ul";
						}
					else if(line=="span") {
						checkBR();
						f2.WriteStringLn("{ ");    // differenzio per checkBR
						onepost.content += "{";
						}
					else if(line=="/span") {
						checkBR();
						f2.WriteStringLn("}");
						onepost.content += "}";
						}
					else if(line=="ul") {
						f2.WriteString("{\\\\*\\listtable{\\list\\listtemplateid68157441\\listsimple");
						f2.WriteString("{\\listlevel\\levelnfc23\\leveljc0\\levelfollow0\\levelstartat1\\levelspace0\\levelindent0");
						f2.WriteString("{\\leveltext\\'01\\u-3913 ?;}");
						f2.WriteString("{\\levelnumbers;}\\f3\\fbias0 \\fi-360\\li360\\jclisttab\\tx360 }");
						f2.WriteString("{\\listname ;}");
						f2.WriteString("\\listid161094266}");
						f2.WriteString("{\\list\\listtemplateid68157455\\listsimple");
						f2.WriteString("{\\listlevel\\levelnfc0\\leveljc0\\levelfollow0\\levelstartat1\\levelspace0\\levelindent0");
						f2.WriteString("{\\leveltext\\'02\\'00.;}");
						f2.WriteString("{\\levelnumbers\\'01;}");
						f2.WriteString("\\fi-360\\li360\\jclisttab\\tx360 }");
						f2.WriteString("{\\listname ;}");
						f2.WriteStringLn("\\listid1522279999}}");
						f2.WriteString("{\\\\*\\listoverridetable");
						f2.WriteStringLn("{\\listoverride\\listid1522279999\\listoverridecount0\\ls1};");
						f2.WriteStringLn("{\\listoverride\\listid161094266\\listoverridecount0\\ls2}");

						onepost.content += "<ul>";  // finire
						}
					else if(line=="/ul") {
						f2.WriteStringLn("}");
						onepost.content += "}";
						}
					else if(line=="li") {
						f2.WriteStringLn("{\\pntext\\pard\\plain\\f3\\fs20\\cgrid \\loch\\af3\\dbch\\af0\\hich\\f3 \\\\'b7\\tab}\\pard \\fi-360\\li360\\widctlpar\\jclisttab\\tx360{\\\\*\\pn \\pnlvlblt\\ilvl0\\ls2\\pnrnot0\\pnf3\\pnstart1\\pnindent360\\pnhang{\\pntxtb \\\\'b7}}\\ls2\\adjustright {");
	//            onepost.content = onepost.content + "{\\pntext\\pard\\plain\\f3\\fs20\\cgrid \\loch\\af3\\dbch\\af0\\hich\\f3 \\\\'b7\\tab}\\pard \\fi-360\\li360\\widctlpar\\jclisttab\\tx360{\\\\*\\pn \\pnlvlblt\\ilvl0\\ls2\\pnrnot0\\pnf3\\pnstart1\\pnindent360\\pnhang{\\pntxtb \\\\'b7}}\\ls2\\adjustright {"
						}
					else if(line=="/li") {
						f2.WriteStringLn("}");
						onepost.content += "}";
						}
					else if(line=="dc:creator") {
						inskip = TRUE;
						}
					else if(line=="/dc:creator") {
						inskip = FALSE;
						}

					else {
						if(line.Left(2) == "wp")
							inwp = TRUE;
						if(line.Left(3) == "/wp")
							inwp = FALSE;
						if(line.Left(4) == "guid")
							inskip = TRUE; // il link interno
						if(line.Left(5) == "/guid")
							inskip = FALSE;
						if(intitle && line.Left(8) == "![CDATA[") {
							if(!first) {
								if(postdate != oldpostdate) {
									oldpostdate = postdate;
									sprintf(myBuf2,"\\page \n{\\*\\bkmkstart %s}\\tab{\\*\\bkmkend %s}",
										(LPCTSTR)monthref,(LPCTSTR)monthref);
									f2.WriteString(myBuf2);
									}
								else
									f2.WriteString("\\pard \\qj\\widctlpar\\adjustright");
								}
							if(first)
								first = FALSE;

//							onepost.title.Empty();     // ...
							onepost.title = line.Mid(8);   // ]] finali...
							{
							CString S=onepost.title; S.TrimRight();
							sprintf(myBuf2,"{\\pard %s }",(LPCTSTR)S);
							f2.WriteStringLn(myBuf2);
							}

								
							onepost.date.Empty();
							onepost.categories.Empty();
							onepost.link.Empty();
							onepost.author.Empty();
							onepost.content.Empty();
							onecomment.content.Empty();
							onecomment.date.Empty();
							onecomment.author.Empty();

							line.Empty();
							*myBuf=0;
							}		// intitle

						if(line.Left(6) == "a href") {
							checkBR();
							{
							sprintf(myBuf2,"{\\field{\\\\*\\fldinst { HYPERLINK %s",(LPCTSTR)line.Mid(7));
							f2.WriteString(myBuf2);
							}
							f2.WriteString("}}}{\\fldrst {\\cs15\\ul\\cf2 ");
					 //     onepost.content = onepost.content + "{\\field{\\\\*\\fldinst { HYPERLINK " + MID$(line$, 8) + "}}}{\\fldrst {\\cs15\\ul\\cf2 "
							}
						if(line == "/a") {
							checkBR();
							f2.WriteString("}}");
							onepost.content += "}}";
							}
						if(line.Left(9) == "div class") {   // x youtube link, ev finire
							checkBR();
							{
							sprintf(myBuf2,"{\\field{\\\\*\\fldinst { HYPERLINK %s }{{\\\\*\\datafield ",(LPCTSTR)line);
							f2.WriteString(myBuf2);
							//f2.WriteStringLn("00d0c9ea79f9bace118c8200aa004ba90b02000000170000001a000000680074007400700073003a002f002f007700770077002e00660061006300650062006f006f006b002e0063006f006d002f000000e0c9ea79f9bace118c8200aa004ba90b4c000000680074007400700073003a002f002f007700770077002e006600"
							//f2.WriteStringLn("61006300650062006f006f006b002e0063006f006d002f000000795881f43b1d7f48af2c825dc485276300000000a5ab0000}}}{\\fldrslt {\\cs15\\ul\\cf2 "; line$;       ' "}}}"
							sprintf(myBuf2,"0000}}}{\\fldrslt {\\cs15\\ul\\cf2 %s",(LPCTSTR)line);       // "}}}");
							f2.WriteString(myBuf2);
							}

		//              onepost.content = onepost.content + "{\\field{\\\\*\\fldinst { HYPERLINK " + line$   ' + "}}}"  ' finire..
							}
						if(line.Left(9) == "div style") {   // in pratica inutili
							checkBR();
							f2.WriteString("{{{style");
							}
						if(line.Left(6) == "div id") {   // in pratica inutili
							checkBR();
							f2.WriteString("{{{id");
							}
						if(line.Left(4) == "/div") {
							checkBR();
							f2.WriteString("}}}");
							onepost.content += "}}}";
							}
						if(line.Left(8) == "category") {  // tag/categorie
							checkBR();
							a = line.Mid(37); a = a.Left(a.GetLength() - 1);
							if(a != "uncategorized") {
								{
								sprintf(myBuf2,"{\\fs18\\ql\\widctlpar\\adjustright Categoria %s; \\par }",(LPCTSTR)a);  // controllare se non e' la prima e togliere la parola!
								f2.WriteString(myBuf2);
								}
								onepost.categories += ";" + a;
								}
							}
						if(line.Left(9) == "/category") {
							 
							}

						if(line.Left(8) == "ul class") {   // diciamo patch per ora
							f2.WriteString("{");
							}

						if(incomment) {
//							if(line.GetLength() > 10) { // patch memory porcodio TOGLIEREEEEE
//								onecomment.content = line.Mid(8);
//								}
//							else
								onecomment.content = line;
							{
							CString S=onecomment.content; S.TrimRight();
							sprintf(myBuf2,"\\li367 COMMENTO: %s \\pard ",(LPCTSTR)S);
							f2.WriteString(myBuf2);
							}
							}
						if(incomment2) {
							onecomment.date = line;
							{
							sprintf(myBuf2,"\\li567 %s; \\pard ",(LPCTSTR)onecomment.date);    //spariscono i trattini???
							f2.WriteString(myBuf2);
							}
							}
						if(incomment3) {
							onecomment.author = line.Mid(8);
							CString S=onecomment.author; S.TrimRight();
							sprintf(myBuf2,"\\li767 %s \\pard ",(LPCTSTR)S);
							f2.WriteString(myBuf2);
							}
						}		// else (default)
					}
					}
					
				intag = FALSE;
				line.Empty();
				break;

			default:
				switch(myBuf[0]) {
					case '\xC3':
						f1.Read(myBuf,1);
						switch(myBuf[0]) {
							case '\xB2':
								a = "o'";
								break;
							case '\xB9':
								a = "u'";
								break;
							case '\xA0':
								a = "a'";
								break;
							case '\xA8':
								a = "e'";
								break;
							case '\xA9':     // perche'
								a = "e'";
								break;
							case '\xAC':
								a = "i'";
								break;
							}
						break;
					case '\xE2':
						f1.Read(myBuf,1);  // 80h fisso
						f1.Read(myBuf,1);
						switch(myBuf[0]) {
							case '\x99':
								a = "'";
								break;
							case '\x9C':
								a = "?";
								break;
							case '\x9D':
								a = "?";
								break;
							case '\xA6':
								a = "?";
								break;
							}
						break;
					}

				//PRINT LEN(line$), SEEK(1)
				//if LEN(line$) > 4000 AND incomment% = FALSE AND intag% = FALSE AND inwp% = FALSE {
	//			if(line.GetLength() > 2000) {
	//				f2.WriteStringLn(line); line.Empty();  //' patch out of string space coglioni TOGLIERE
	//				}
				line += a;
				if(!intag) {
					if(!inwp) {

						}
					}
				break;

			}		// (main) switch

		if((f1.GetPosition() / cntDivider) != precpos) {
			//LOCATE 3, 1
			printf("\r%3.1f%%    ", ((double)f1.GetPosition() / tot) * 100);
			precpos = f1.GetPosition()/cntDivider;
			}

		}  //while

	
fine:
	f2.WriteStringLn("}");

	f2.Close();

	f1.Close();

	puts("\nFinito.");
	return nRetCode;
	}


CTimeEx calcdate(CString a) {
	int dYear = atoi(a.Mid(12,4));
	int dDay = atoi(a.Mid(5,2));
//  PRINT dYear%, dDay%

	int i=CTimeEx::getMonthFromString(a.Mid(8,3));
	CTimeEx t(dYear,i,dDay,0,0,0);
	return t;
	}

void checkBR() {
	char myBuf[16];

int i=	f2.Seek(-6,CFile::current);
//  EXIT SUB
	f2.Read(myBuf,4);
	myBuf[4]=0;
	if(!strcmp(myBuf,"\\par")) {
		f2.Seek(-6,CFile::current);
		f2.WriteStringLn();
		}
	else {
//		f2.Seek(0,CFile::current);
		f2.SeekToEnd();
		}
	
	}

CString getdate() {

	return CTime::GetCurrentTime().Format("\\yr%Y\\mo%m\\dy%d\\hr%H\\min%M");
	}


// -------------------------------------------------------------------------------------------------
CString CStringEx::Tokenize(CString delimiter, int& first) {
  CString token;
  int end = Find(delimiter, first);

  if(end != -1) {
    int count = end-first;
    token = Mid(first,count);
    first = end+delimiter.GetLength();
    return token;
	  }
  else {
    int count = GetLength() - first;
    if(count <= FALSE)
      return "";

    token = Mid(first,count);
    first = GetLength();
    return token;
		}
	}
CStringEx CStringEx::SubStr(int begin, int len) const {
	return CString::Mid(begin, len);
	}
int CStringEx::FindNoCase(CString substr,int start) {
	CString s1=*this,s2;
	s1.MakeUpper();
	s2=substr;
	s2.MakeUpper();
	return s1.Find(s2,start);
	}
CStringEx::CStringEx(int i, const char *format, DWORD options) {

	Format(format,i);
	if(options & COMMA_DELIMIT)
		CStringEx::operator=(CommaDelimitNumber(*this));
	}
CStringEx::CStringEx(double d, const char *format, DWORD options) {

	Format(format,d);
	if(options & COMMA_DELIMIT)
		CStringEx::operator=(CommaDelimitNumber(*this));
	}
CStringEx CStringEx::CommaDelimitNumber(const char *s) {
	CStringEx s2=s;												// convert to CStringEx
	return CommaDelimitNumber(s2);
	}
CStringEx CStringEx::CommaDelimitNumber(CString s2) {
	CStringEx dp;
	CStringEx q2;											// working string
	CStringEx posNegChar=s2.Left(1);				// get the first char
	bool posNeg=!posNegChar.IsDigit(0);			// if not digit, then assume + or -

	if(posNeg) 											// if so, strip off
		s2=s2.Mid(1);
	if(s2.Find(decimalChar)>=0) {
		dp=s2.Mid(s2.Find(decimalChar)+1);							// remember everything to the right of the decimal point
		s2=s2.Left(s2.Find(decimalChar));				// get everything to the left of the first decimal point
		}
	while(s2.GetLength() > 3) {									// if more than three digits...
		CStringEx s3(thousandChar);
		s3+=s2.Right(3);		// insert a comma before the last three digits (100's)
		q2=s3+q2;											// append this to our working string
		s2=s2.Left(s2.GetLength()-3);							// get everything except the last three digits
		}
	q2=s2+q2;												// prepend remainder to the working string
	if(!dp.IsEmpty()) {									// if we have decimal point...
		q2+=decimalChar;							// append it and the digits
		q2+=dp;							// append it and the digits
		}
	if(posNeg)											// if we stripped off a +/- ...
		q2=posNegChar+q2;			// add it back in

	return q2;											// this is our final comma delimited string
	}

CStringEx CStringEx::CommaDelimitNumber(DWORD n) {
	CStringEx q2;

	q2.Format("%u",n);
	q2=CommaDelimitNumber(q2);
	return q2;
	}

BYTE CStringEx::Asc(int pos) {

	return GetAt(pos);
	}

int CStringEx::Val(int base) {

	switch(base) {
		case 10:
		default:
			return atoi((LPCTSTR)this);
			break;
		case 16:		// fare...
			break;
		}
	}

double CStringEx::Val() {

	return strtod((LPCTSTR)this,NULL);
	}

void CStringEx::Repeat(int n) {
	CString s2=*this;

	Empty();
	while(n--)
		CStringEx::operator+=(s2);
	}

void CStringEx::Repeat(const char *s,int n) {

	Empty();
	while(n--)
		CStringEx::operator+=(s);
	}

void CStringEx::Repeat(char c,int n) {

	Empty();
	while(n--)
		CStringEx::operator+=(c);
	}

BOOL CStringEx::IsAlpha(char ch) {

	return (ch>='A' && ch<='Z') || (ch>='a' && ch<='z');
	}

BOOL CStringEx::IsAlpha(int pos) {

	return IsAlpha(GetAt(pos));
	}

BOOL CStringEx::IsAlnum(char ch) {

	return IsAlpha(ch) || IsDigit(ch);
	}

BOOL CStringEx::IsAlnum(int pos) {

	return IsAlnum(GetAt(pos));
	}

BOOL CStringEx::IsDigit(char ch) {

	return (ch>='0' && ch<='9');
	}

BOOL CStringEx::IsDigit(int pos) {

	return IsDigit(GetAt(pos));
	}

BOOL CStringEx::IsPrint(char ch) {

	return (ch>=' ' && ch<'\x7f');			//127 escluso
	}

BOOL CStringEx::IsPrint(int pos) {

	return IsPrint(GetAt(pos));
	}

void CStringEx::Print() {

	AfxMessageBox(*this);
	}

void CStringEx::Debug() {

#ifdef _DEBUG
	Print();
#endif
	}

WORD CStringEx::GetAsciiLength() {			// utile per saltare ESC ecc in stampa citofono LCD ecc
	WORD i,j;

	for(i=0,j=0; i<GetLength(); i++)
		if(IsPrint(i))
			j++;
	return j;
	}

// _T("%h %l %u %t \"%r\" %>s %b")		v. Apache...

CStringEx CStringEx::FormatTime(int m,CTime mT) {

	if(!(*((DWORD *)&mT)))
		mT=CTime::GetCurrentTime();

	switch(m) {
		case 0:
			CStringEx::operator=(mT.Format("%d/%m/%Y %H:%M:%S"));
			break;
		case 1:
			Format(_T("%02u/%s/%02u:%02u:%02u:%02u %02d00"),
				mT.GetDay(),
				CTimeEx::Num2Month3(mT.GetMonth()),
				mT.GetCurrentTime().GetYear(),
				mT.GetCurrentTime().GetHour(),
				mT.GetMinute(),
				mT.GetSecond(),
				-(_timezone/3600)+(_daylight ? 1 : 0)
				);
			break;
		case 2:
			CStringEx::operator=(mT.Format(_T("%a, %d %b %Y %H:%M:%S %Z")));
			break;
		}

	return *this;
	}


const char CStringEx::m_base64tab[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                      "abcdefghijklmnopqrstuvwxyz0123456789+/";
const int CStringEx::BASE64_MAXLINE=76;
const char *CStringEx::EOL="\r\n";
const char CStringEx::decimalChar=',',CStringEx::thousandChar='.';		// GetLocale ??
const char CStringEx::CRchar='\r',CStringEx::LFchar='\n',CStringEx::TABchar='\t';
CStringEx CStringEx::Encode64() {
	CStringEx S2;

  //Set up the parameters prior to the main encoding loop
  int nInPos  = FALSE;
  int nLineLen = FALSE;

  // Get three characters at a time from the input buffer and encode them
  for(int i=0; i<GetLength()/3; ++i) {

    //Get the next 2 characters
    int c1 = Asc(nInPos++) & 0xFF;
    int c2 = Asc(nInPos++) & 0xFF;
    int c3 = Asc(nInPos++) & 0xFF;

    //Encode into the 4 6 bit characters
    S2 += m_base64tab[(c1 & 0xFC) >> 2];
    S2 += m_base64tab[((c1 & 0x03) << 4) | ((c2 & 0xF0) >> 4)];
    S2 += m_base64tab[((c2 & 0x0F) << 2) | ((c3 & 0xC0) >> 6)];
    S2 += m_base64tab[c3 & 0x3F];
    nLineLen += 4;

    //Handle the case where we have gone over the max line boundary
    if(nLineLen >= BASE64_MAXLINE-3) {
      const char *cp = EOL;
      S2 += *cp++;
      if(*cp) {
        S2 += *cp;
				}
      nLineLen = FALSE;
			}
		}

  // Encode the remaining one or two characters in the input buffer
  const char *cp;
  switch(GetLength() % 3) {
    case 0:
      cp = EOL;
      S2 += *cp++;
      if(*cp) {
        S2 += *cp;
				}
      break;
    case 1:
    {
      int c1 = Asc(nInPos) & 0xFF;
      S2 += m_base64tab[(c1 & 0xFC) >> 2];
      S2 += m_base64tab[((c1 & 0x03) << 4)];
      S2 += '=';
      S2 += '=';
      cp = EOL;
      S2 += *cp++;
      if(*cp) {
        S2 += *cp;
				}
      break;
    }
    case 2:
    {
      int c1 = Asc(nInPos++) & 0xFF;
      int c2 = Asc(nInPos) & 0xFF;
      S2 += m_base64tab[(c1 & 0xFC) >> 2];
      S2 += m_base64tab[((c1 & 0x03) << 4) | ((c2 & 0xF0) >> 4)];
      S2 += m_base64tab[((c2 & 0x0F) << 2)];
      S2 += '=';
      cp = EOL;
      S2 += *cp++;
      if(*cp) {
        S2 += *cp;
				}
      break;
    }
    default: 
      ASSERT(FALSE); 
      break;
	  }

  CStringEx::operator=(S2);
  return *this;
	}

int CStringEx::Decode64() {
	CStringEx sInput;
	int m_nBitsRemaining;
	ULONG m_lBitStorage;

  m_nBitsRemaining = FALSE;

	sInput=*this;
  Empty();  
	if(sInput.GetLength() == FALSE)
		return 0;

	//Build Decode Table
  int nDecode[256];
	for(int i=0; i<256; i++) 
		nDecode[i] = -2; // Illegal digit
	for(i=0; i<64; i++) {
		nDecode[m_base64tab[i]] = i;
		nDecode[m_base64tab[i] | 0x80] = i; // Ignore 8th bit
		nDecode['='] = -1; 
		nDecode['=' | 0x80] = -1; // Ignore MIME padding char
		}

	// Decode the Input
  i=0;
  TCHAR* szOutput = GetBuffer(sInput.GetLength());
	for(int p=0; p<sInput.GetLength(); p++) {
		int c = sInput[p];
		int nDigit = nDecode[c & 0x7F];
		if(nDigit < -1) {
      ReleaseBuffer();  
			return 0;
			}
		else if(nDigit >= FALSE) {
			// i (index into output) is incremented by write_bits()
//			WriteBits(nDigit & 0x3F, 6, szOutput, i);
			UINT nScratch;

			m_lBitStorage = (m_lBitStorage << 6) | (nDigit & 0x3F);
			m_nBitsRemaining += 6;
			while(m_nBitsRemaining > 7) {
				nScratch = m_lBitStorage >> (m_nBitsRemaining - 8);
				szOutput[i++] = (TCHAR) (nScratch & 0xFF);
				m_nBitsRemaining -= 8;
				}
			}
		}	
  szOutput[i] = _T('\0');
  ReleaseBuffer();

	return i;
	}

CString CStringEx::InsertSeparator(DWORD dwNumber) {

  Format("%u", dwNumber);
  
  for(int i=GetLength()-3; i > 0; i -= 3) {
    Insert(i, ",");
    }

  return *this;
  }

CStringEx CStringEx::FormatSize(DWORD dwFileSize) {
  static const DWORD dwKB = 1024;          // Kilobyte
  static const DWORD dwMB = 1024 * dwKB;   // Megabyte
  static const DWORD dwGB = 1024 * dwMB;   // Gigabyte

  DWORD dwNumber, dwRemainder;

  if(dwFileSize < dwKB) {
//    InsertSeparator(dwFileSize) + " B";		// non funziona (usare  *this o Format) e poi non mi piace!
    InsertSeparator(dwFileSize);
		} 
  else {
    if(dwFileSize < dwMB) {
      dwNumber = dwFileSize / dwKB;
      dwRemainder = (dwFileSize * 100 / dwKB) % 100;

      Format("%s.%02d KB", (LPCSTR)InsertSeparator(dwNumber), dwRemainder);
			}
    else {
      if(dwFileSize < dwGB) {
        dwNumber = dwFileSize / dwMB;
        dwRemainder = (dwFileSize * 100 / dwMB) % 100;
        Format("%s.%02d MB", InsertSeparator(dwNumber), dwRemainder);
				}
      else {
        if(dwFileSize >= dwGB) {
          dwNumber = dwFileSize / dwGB;
          dwRemainder = (dwFileSize * 100 / dwGB) % 100;
          Format("%s.%02d GB", InsertSeparator(dwNumber), dwRemainder);
					}
				}
			}
		}

  // Display decimal points only if needed
  // another alternative to this approach is to check before calling str.Format, and 
  // have separate cases depending on whether dwRemainder == FALSE or not.
  Replace(".00", "");

	return *this;
	}


// ---------------------------------------------------------------------------
CString CTimeEx::getNow(int ex) {
	int i;
	CString S;

	switch(ex) {
		case 0:
			S=CTime::GetCurrentTime().Format("%d/%m/%Y %H:%M:%S");
			break;
		case 1:
		case 2:
			S.Format(ex == TRUE ? _T("sono le ore %d e %d del %d %s %d") : _T("sono le ore %02d e %02d del %d %s %d"),
				GetCurrentTime().GetHour(),GetCurrentTime().GetMinute(),
				GetCurrentTime().GetDay(),(LPTSTR)(LPCTSTR)Num2Mese(GetCurrentTime().GetMonth()),
				GetCurrentTime().GetYear());
			break;
		}

	return S;
  }


CString CTimeEx::getNowGMT(BOOL bAddCR) {
	time_t aclock;
	struct tm *newtime;
	int i;
	CString S;

	S.Format(_T("%s %s %02u %02u:%02u:%02u %04u"),
		Num2Day3(CTime::GetCurrentTime().GetDayOfWeek()),
		Num2Month3(CTime::GetCurrentTime().GetMonth()),
		CTime::GetCurrentTime().GetDay(),
		CTime::GetCurrentTime().GetHour(),
		CTime::GetCurrentTime().GetMinute(),
		CTime::GetCurrentTime().GetSecond(),
		CTime::GetCurrentTime().GetYear());

#ifndef _WIN32_WCE
	i=-(_timezone/3600)+(_daylight ? 1 : 0);
#else
	i=0;
#endif
	if(!i)
		S+=_T(" UTC");				// anche "GMT"
	else {
		CString S2;
		S2.Format(_T(" %c%02d00"),i>=0 ? '+' : '-',abs(i));
		S+=S2;
		}
	if(bAddCR)
		S+="\r\n";

	return S;
	}

CString CTimeEx::getNowGoogle(BOOL bAddCR) {
	time_t aclock;
	struct tm *newtime;
	int i;
	CString S;

	S.Format(_T("%04u-%02u-%02uT%02u:%02u:%02uZ"),
		CTime::GetCurrentTime().GetYear(),
		CTime::GetCurrentTime().GetMonth(),
		CTime::GetCurrentTime().GetDay(),
		CTime::GetCurrentTime().GetHour(),
		CTime::GetCurrentTime().GetMinute(),
		CTime::GetCurrentTime().GetSecond());

#ifndef _WIN32_WCE
	i=-(_timezone/3600)+(_daylight ? 1 : 0);
#else
	i=0;
#endif
	if(!i)
		S+=_T(" UTC");				// anche "GMT"
	else {
		CString S2;
		S2.Format(_T(" %c%02d00"),i>=0 ? '+' : '-',abs(i));
		S+=S2;
		}
	if(bAddCR)
		S+="\r\n";

	return S;
	}

CString CTimeEx::Num2Mese(int i) {

  switch(i) {
		case 1:
      return _T("Gennaio");
			break;
		case 2:
      return _T("Febbraio");
			break;
		case 3:
      return _T("Marzo");
			break;
		case 4:
      return _T("Aprile");
			break;
		case 5:
      return _T("Maggio");
			break;
		case 6:
      return _T("Giugno");
			break;
		case 7:
      return _T("Luglio");
			break;
		case 8:
      return _T("Agosto");
			break;
		case 9:
	    return _T("Settembre");
			break;
		case 10:
      return _T("Ottobre");
			break;
		case 11:
      return _T("Novembre");
			break;
		case 12:
      return _T("Dicembre");
			break;
	  }
  
  }

CString CTimeEx::Num2Giorno(int i) {

  switch(i) {
		case 1:
      return _T("Domenica");
			break;
		case 2:
      return _T("Lunedì");
			break;
		case 3:
      return _T("Martedì");
			break;
		case 4:
      return _T("Mercoledì");
			break;
		case 5:
      return _T("Giovedì");
			break;
		case 6:
      return _T("Venerdì");
			break;
		case 7:
      return _T("Sabato");
			break;
	  }
  }

CString CTimeEx::Num2Month3(int i) {

  switch(i) {
		case 1:
      return _T("Jan");
			break;
		case 2:
      return _T("Feb");
			break;
		case 3:
      return _T("Mar");
			break;
		case 4:
      return _T("Apr");
			break;
		case 5:
      return _T("May");
			break;
		case 6:
      return _T("Jun");
			break;
		case 7:
      return _T("Jul");
			break;
		case 8:
      return _T("Aug");
			break;
		case 9:
	    return _T("Sep");
			break;
		case 10:
      return _T("Oct");
			break;
		case 11:
      return _T("Nov");
			break;
		case 12:
      return _T("Dec");
			break;
	  }
  
  }

CString CTimeEx::Num2Day3(int i) {

  switch(i) {
		case 1:
      return _T("Sun");
			break;
		case 2:
      return _T("Mon");
			break;
		case 3:
      return _T("Tue");
			break;
		case 4:
      return _T("Wed");
			break;
		case 5:
      return _T("Thu");
			break;
		case 6:
      return _T("Fri");
			break;
		case 7:
      return _T("Sat");
			break;
	  }
  }

int CTimeEx::getMonthFromString(const CString S) {

	if(!S.CompareNoCase(_T("JAN")))
		return 1;
	else if(!S.CompareNoCase(_T("FEB")))
		return 2;
	else if(!S.CompareNoCase(_T("MAR")))
		return 3;
	else if(!S.CompareNoCase(_T("APR")))
		return 4;
	else if(!S.CompareNoCase(_T("MAY")))
		return 5;
	else if(!S.CompareNoCase(_T("JUN")))
		return 6;
	else if(!S.CompareNoCase(_T("JUL")))
		return 7;
	else if(!S.CompareNoCase(_T("AUG")))
		return 8;
	else if(!S.CompareNoCase(_T("SEP")))
		return 9;
	else if(!S.CompareNoCase(_T("OCT")))
		return 10;
	else if(!S.CompareNoCase(_T("NOV")))
		return 11;
	else if(!S.CompareNoCase(_T("DEC")))
		return 12;

	return 0;
	}

int CTimeEx::getMonthFromGMTString(const CString S) {
	CString S2=S.Left(3);

	return getMonthFromString(S2);

	}


CString CTimeEx::getFasciaDellaGiornata() {

	if(GetCurrentTime().GetHour() >=7 && GetCurrentTime().GetHour()<13)
		return _T("stamattina");
	else if(GetCurrentTime().GetHour() >=13 && GetCurrentTime().GetHour()<20)
		return _T("oggi");
	else if(GetCurrentTime().GetHour() >=20 && GetCurrentTime().GetHour()<24)
		return _T("stasera");
	else if(GetCurrentTime().GetHour() >=0 && GetCurrentTime().GetHour()<7)
		return _T("stanotte");

	}

CString CTimeEx::getSaluto() {

	if(GetCurrentTime().GetHour() >=7 && GetCurrentTime().GetHour()<13)
		return _T("Buongiorno");
	else if(GetCurrentTime().GetHour() >=13 && GetCurrentTime().GetHour()<20)
		return _T("Buon pomeriggio");
	else if(GetCurrentTime().GetHour() >=20 && GetCurrentTime().GetHour()<24)
		return _T("Buonasera");
	else if(GetCurrentTime().GetHour() >=0 && GetCurrentTime().GetHour()<7)
		return _T("Buonanotte");

	}

CTime CTimeEx::parseGMTTime(const CString S) {
	char *p;
	int i,j,tzFound=0,reverseUTC=0;
	struct tm t;
	CString s=S;
	
//	_tzset();			// questo imposterebbe la timezone, che altrimenti potrebbe defaultare a -8h
	// v. Joshua.cpp::InitInstance

	while(_istspace(s.GetAt(0)))
		s=s.Mid(1);
	if(_istalpha(s.GetAt(0))) {
		s.MakeUpper();
		if(!s.Left(2).CompareNoCase(_T("SU")))
			i=0;
		else if(!s.Left(2).CompareNoCase(_T("MO")))
			i=1;
		else if(!s.Left(2).CompareNoCase(_T("TU")))
			i=2;
		else if(!s.Left(2).CompareNoCase(_T("WE")))
			i=3;
		else if(!s.Left(2).CompareNoCase(_T("TH")))
			i=4;
		else if(!s.Left(2).CompareNoCase(_T("FR")))
			i=5;
		else if(!s.Left(2).CompareNoCase(_T("SA")))
			i=6;
		else				// NON deve capitare... patch per evitare il peggio!
			i=0;
		t.tm_wday=i;
		s=s.Mid(3);
		while(!isspace(s.GetAt(0)))
			s=s.Mid(1);
		if(s.GetAt(0) ==',')
			s=s.Mid(1);
		s=s.Mid(1);
no_day:
		while(_istspace(s.GetAt(0)))
			s=s.Mid(1);
		if(_istdigit(s.GetAt(0))) {
			t.tm_mday=_ttoi((LPTSTR)(LPCTSTR)s);
			while(iswdigit(s.GetAt(0)))
				s=s.Mid(1);
			s=s.Mid(1);
			t.tm_mon=getMonthFromGMTString(s);
			s=s.Mid(4);
			t.tm_year=_ttoi((LPTSTR)(LPCTSTR)s);
			if(t.tm_year<80)
				t.tm_year+=100;
			if(t.tm_year>=200)
				t.tm_year-=1900;
			s=s.Mid(5);
			t.tm_hour=_ttoi(s);
			s=s.Mid(3);
			t.tm_min=_ttoi(s);
			s=s.Mid(3);
			t.tm_sec=_ttoi(s);
			if(s.GetAt(1) == '-')
				reverseUTC=1;
			s=s.Mid(2,2);
			i=_ttoi(s);
			if(reverseUTC)
				i=-i;
			}
		else {
			t.tm_mon=getMonthFromGMTString(s);
			s=s.Mid(4);
			t.tm_mday=_ttoi(s);
			s=s.Mid(3);
			t.tm_hour=_ttoi(s);
			s=s.Mid(3);
			t.tm_min=_ttoi(s);
			s=s.Mid(3);
			t.tm_sec=_ttoi(s);
			s=s.Mid(3);
			if(s.GetAt(0) == 'U') {		// variante con "UTC 1998
				t.tm_year=_ttoi(s.Mid(4))-1900;
				i=0;								// greenwich
				tzFound=1;
				}
			else {							  // variante con 1998 +0100
				t.tm_year=_ttoi(s)-1900;
				s=s.Mid(4);
				while(s.GetLength()>0 && _istspace(s.GetAt(0)))
					s=s.Mid(1);
				if(s.GetLength()>0) {		// se la timezone segue l'anno...
					if(_istdigit(s.GetAt(0)) || s.GetAt(0)=='+' || s.GetAt(0)=='-') {
						tzFound=1;
						if(s.GetAt(0) == '-')
							reverseUTC=1;
						if(s.GetAt(0)=='+' || s.GetAt(0)=='-')
							s=s.Mid(1);
						s=s.Mid(0,2);		// stronco i minuti!
						i=_ttoi(s);									// ...la leggo
						if(reverseUTC)
							i=-i;
						}
					else if(S.GetAt(0)=='U' || S.GetAt(0)=='G')	{		// per ora solo UTC o GMT (sono lo stesso?)!
						i=0;								// greenwich
						tzFound=1;
						}
					}
				}
			}
		}
	else {
		if(_istalpha(s.GetAt(4))) {		// caso in cui manca il giorno della settimana, poi idem come sopra...
			s.MakeUpper();
			t.tm_wday=0;
			goto no_day;
			}
		else {
			s=s.Mid(2);		// salto \xd\xa
			s=s.Mid(6);
			t.tm_year=_ttoi((LPTSTR)(LPCTSTR)s);
			s=s.Mid(3);
			t.tm_mon=_ttoi((LPTSTR)(LPCTSTR)s);
			s=s.Mid(3);
			t.tm_mday=_ttoi((LPTSTR)(LPCTSTR)s);
			s=s.Mid(3);
			t.tm_hour=_ttoi((LPTSTR)(LPCTSTR)s);
			s=s.Mid(3);
			t.tm_min=_ttoi((LPTSTR)(LPCTSTR)s);
			s=s.Mid(3);
			t.tm_sec=_ttoi((LPTSTR)(LPCTSTR)s);
			s=s.Mid(3);
			i=_ttoi((LPTSTR)(LPCTSTR)s);
			}
		}
	if(tzFound)
#ifndef _WIN32_WCE
		t.tm_hour=t.tm_hour-i-(_timezone/3600)+(_daylight ? 1 : 0);
#else
		t.tm_hour=t.tm_hour-i-(0/3600)+0;
#endif

	return CTime::CTime(t.tm_year+1900,t.tm_mon,t.tm_mday,t.tm_hour,t.tm_min,t.tm_sec);
	}

CTime CTimeEx::parseTime(const CString S) {		// semplice, tipo "05/10/10 12:00"
	char *p;
	int i,j,tzFound=0;
	WORD nDay,nMonth,nYear,nHour,nMinute,nSecond;
	CString s=S;
	
//	_tzset();			// questo imposterebbe la timezone, che altrimenti potrebbe defaultare a -8h
	// v. Joshua.cpp::InitInstance

	p=(char *)(LPCTSTR)S;
	while(isspace(*p) && *p)
		p++;
	nDay=atoi(p);		// 2 digit
	while(isdigit(*p))
		p++;
	p++;
	while(isspace(*p) && *p)
		p++;
	nMonth=atoi(p);			// 2 digit
	while(isdigit(*p))
		p++;
	p++;
	while(isspace(*p) && *p)
		p++;
	nYear=atoi(p);		// 2-4 digit (v.sotto)
	while(isdigit(*p))
		p++;

	nHour=0;
	nMinute=0;
	nSecond=0;
	if(*p) {
		p++;
		while(isspace(*p) && *p)
			p++;
		nHour=atoi(p);		// 2 digit
		while(isdigit(*p))
			p++;
		p++;
		while(isspace(*p) && *p)
			p++;
		nMinute=atoi(p);	// 2 digit

		while(isdigit(*p))
			p++;
		if(*p) {
			p++;
			while(isspace(*p) && *p)
				p++;
			nSecond=atoi(p); // 2 digit
			}
		}

	if(nYear < 100) {
		if(nYear < 80)
			nYear+=2000;
		else
			nYear+=1900;
		}


	i=0;
	if(tzFound)
#ifndef _WIN32_WCE
		nHour=nHour-i-(_timezone/3600)+(_daylight ? 1 : 0);
#else
		nHour=nHour-i-(0/3600)+0;
#endif

	return CTime::CTime(nYear,nMonth,nDay,nHour,nMinute,nSecond);
	}

BOOL CTimeEx::isWeekend() {

	return GetCurrentTime().GetDayOfWeek() == TRUE || GetCurrentTime().GetDayOfWeek() == 7;
	}

BOOL CTimeEx::isWeekend(CTime t) {

	return t.GetDayOfWeek() == TRUE || t.GetDayOfWeek() == 7;
	}

WORD CTimeEx::GetDayOfYear() {
	struct tm *myTm;

	myTm=CTime::GetCurrentTime().GetLocalTm();
	return myTm->tm_yday;
	}

