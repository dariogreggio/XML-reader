DECLARE SUB checkBR ()
DECLARE FUNCTION calcdate (a$)
DECLARE FUNCTION getdate$ ()

' converter for Wordpress Export XML files to RTF, so that the idiots over there won't cause any more damage blocking blogs for no reason
' G.Dar 2/7/2023-26/7/2023

' https://www.biblioscape.com/rtf15_spec.htm

CLS
DIM SHARED of$

TYPE post
	title AS STRING * 128
	date AS STRING * 16
	author AS STRING * 32
	content AS STRING * 8000
	link AS STRING * 128
	categories AS STRING * 160
END TYPE
TYPE comment
	date AS STRING * 16
	author AS STRING * 32
	content AS STRING * 512
END TYPE

DIM onepost AS post, onecomment AS comment

if$ = "wp.xml": of$ = "wpout.rtf"
PRINT "Conversione file "; if$

OPEN if$ FOR INPUT AS 1
OPEN of$ FOR OUTPUT AS 2

tot& = LOF(1)

first% = 1
PRINT #2, "{\rtf1\ansi\ansicpg1252\uc1 \deff0\deflang1033\deflangfe1040"
'pca850
PRINT #2, "{\fonttbl"
PRINT #2, "{\f0\froman\fcharset0\fprq2{\*\panose 02020603050405020304}Times New Roman;}"
PRINT #2, "{\f1\fswiss\fcharset0\fprq2{\*\panose 020b0604020202020204}Arial;}"
'PRINT #2, "{\f16\froman\fcharset238\fprq2 Times New Roman CE;}"
'PRINT #2, "{\f17\froman\fcharset204\fprq2 Times New Roman Cyr;}"
'PRINT #2, "{\f19\froman\fcharset161\fprq2 Times New Roman Greek;}"
'PRINT #2, "{\f20\froman\fcharset162\fprq2 Times New Roman Tur;}"
'PRINT #2, "{\f21\froman\fcharset186\fprq2 Times New Roman Baltic;}"
'PRINT #2, "{\f22\fswiss\fcharset238\fprq2 Arial CE;}"
'PRINT #2, "{\f23\fswiss\fcharset204\fprq2 Arial Cyr;}"
'PRINT #2, "{\f25\fswiss\fcharset161\fprq2 Arial Greek;}"
'PRINT #2, "{\f26\fswiss\fcharset162\fprq2 Arial Tur;}"
'PRINT #2, "{\f27\fswiss\fcharset186\fprq2 Arial Baltic;}";
PRINT #2, "}"
PRINT #2, "{\colortbl;\red0\green0\blue0;\red0\green0\blue255;\red0\green255\blue255;\red0\green255\blue0;\red255\green0\blue255;\red255\green0\blue0;\red255\green255\blue0;\red255\green255\blue255;\red0\green0\blue128;";
PRINT #2, "\red0\green128\blue128;\red0\green128\blue0;\red128\green0\blue128;\red128\green0\blue0;\red128\green128\blue0;\red128\green128\blue128;\red192\green192\blue192;}"
PRINT #2, "{\stylesheet{\widctlpar\adjustright \fs20\lang1040\cgrid \snext0 Normal;}"
PRINT #2, "{\s1\sb240\sa60\keepn\widctlpar\adjustright \b\f1\fs28\lang1040\kerning28\cgrid \sbasedon0 \snext0 heading 1;}"
PRINT #2, "{\*\cs10 \additive Default Paragraph Font;}"
PRINT #2, "{\*\cs15 \additive \ul\cf2 \sbasedon10 Hyperlink;}}"


PRINT #2, "{\info"; "{\title "; if$; " converted by G.Dar converter on "; DATE$; "}";
PRINT #2, "{\author Dario Greggio}"; "{\operator dario Greggio}";
PRINT #2, "{\creatim"; getdate$; "}{\revtim"; getdate$; "}";
PRINT #2, "{\version1}{\edmins1}{\nofpages1000}{\nofwords1000000}{\nofchars1000000}{\nofcharsws1000000}";
' FINIRE con conteggio!
PRINT #2, "{\*\company Dario's Automation which fucks wordpress and automattic of shit}";
PRINT #2, "{\vern1}}"

WHILE NOT EOF(1)
	a$ = INPUT$(1, 1)
	IF a$ = "" THEN GOTO fine
 
	SELECT CASE ASC(a$)
		CASE 60
			IF intag% = 0 THEN
				IF inwp% = 0 THEN
					IF inskip% = 0 AND line$ <> "" THEN
						IF ASC(line$) <> 10 THEN
							IF LEFT$(line$, 2) <> "]]" THEN
								PRINT #2, line$;
'\pard\plain \s1\sb240\sa60\keepn\widctlpar\outlinelevel0\adjustright \b\f1\fs28\lang1040\kerning28\cgrid {
								onepost.content = onepost.content + line$
							ELSE
								PRINT #2, "\par "
							END IF
						
							IF inimage% = 0 AND inlink% = 0 AND intitle% = 0 THEN PRINT #2, "\par " ELSE PRINT #2,
							IF indate% THEN
								onepost.date = line$
								postdate = calcdate(line$)
								monthref$ = MID$(line$, 9, 8)
							END IF
							IF inlink% THEN
								onepost.link = line$
							END IF
						END IF
					END IF
				ELSE

				END IF
			END IF

			intag% = 1
			line$ = ""

		CASE 62
			IF intag% = 0 THEN
				IF inwp% = 0 THEN
					IF inskip% = 0 AND line$ <> "" THEN
						IF ASC(line$) <> 10 THEN
							IF LEFT$(line$, 2) <> "]]" THEN
								PRINT #2, line$;

								onepost.content = onepost.content + line$
							ELSE
								PRINT #2, "\par "
							END IF
							IF inimage% = 0 AND inlink% = 0 AND intitle% = 0 THEN PRINT #2, "\par " ELSE PRINT #2,
						END IF
					END IF
				ELSE

				END IF
			ELSE
				SELECT CASE line$
					CASE "p"
						PRINT #2, "{\qj "
					CASE "/p"
						PRINT #2, "}"
					CASE "title"
						IF initem% = 0 THEN
							PRINT #2, "{\pard\fs36\i\b "    'PRINT #2, "{\title "
'PRINT #2, "{\info";"{\title ";
						ELSE
							PRINT #2, "{\pard\fs28\ul "
							IF onepost.categories <> "" OR onecomment.content <> "" THEN PRINT #2, "\par "
						END IF
						intitle% = 1
					CASE "/title"
						IF initem% = 0 THEN
							PRINT #2, "}"
						' FINIRE esce anche con IMG SRC..
						ELSE
							PRINT #2, "}"
						END IF

'           {\version2}
'           {\edmins1}
'           {\nofpages2}
'           {\nofwords271}
'           {\nofchars1547}
'           {\*\company oetztal automation}
'           {\nofcharsws1899}
'           {\vern89}
'           }\paperw11906\paperh16838\margl1134\margr1134\margt1417\margb1134
'\deftab708\widowctrl\ftnbj\aenddoc\hyphhotz283\formshade\viewkind1\viewscale100\pgbrdrhead\pgbrdrfoot \fet0\sectd \linex0\headery709\footery709\colsx709\endnhere\sectdefaultcl
'{\*\pnseclvl1\pnucrm\pnstart1\pnindent720\pnhang{\pntxta .}}
'{\*\pnseclvl2\pnucltr\pnstart1\pnindent720\pnhang{\pntxta .}}
'{\*\pnseclvl3\pndec\pnstart1\pnindent720\pnhang{\pntxta .}}
'{\*\pnseclvl4\pnlcltr\pnstart1\pnindent720\pnhang{\pntxta )}}
'{\*\pnseclvl5\pndec\pnstart1\pnindent720\pnhang{\pntxtb (}{\pntxta )}}
'{\*\pnseclvl6\pnlcltr\pnstart1\pnindent720\pnhang{\pntxtb (}{\pntxta )}}
'{\*\pnseclvl7\pnlcrm\pnstart1\pnindent720\pnhang{\pntxtb (}{\pntxta )}}
'{\*\pnseclvl8\pnlcltr\pnstart1\pnindent720\pnhang{\pntxtb (}{\pntxta )}}
'{\*\pnseclvl9\pnlcrm\pnstart1\pnindent720\pnhang{\pntxtb (}{\pntxta )}
'}\pard\plain \s1\sb240\sa60\keepn\widctlpar\outlinelevel0\adjustright \b\f1\fs28\lang1040\kerning28\cgrid {Il riscrittore di esistenze
'\par }\pard\plain \widctlpar\adjustright \fs20\lang1040\cgrid {

						intitle% = 0
					CASE "image"    ' qua non abbiamo immagini esterne linkate...!
						PRINT #2, "{{\*\shppict{\pict{\*\picprop\shplid1025{\sp{\sn shapeType}{\sv 75}}{\sp{\sn fFlipH}{\sv 0}}{\sp{\sn fFlipV}{\sv 0}}{\sp{\sn pibName}"
'           {\sv \'5c\'5c\'73\'6b\'79\'6e\'65\'74\'5c\'70\'72\'69\'76\'61\'74\'65\'5c\'73\'61\'6e\'74\'6f\'67\'64\'2e\'6a\'70\'65\'67}}
'           {\sp{\sn pibFlags}{\sv 2}}{\sp{\sn fillColor}{\sv 268435473}}{\sp{\sn fFilled}{\sv 0}}{\sp{\sn fLine}{\sv 0}}}
						PRINT #2, "\picscalex100\picscaley100\piccropl0\piccropr0\piccropt0\piccropb0\picw2646\pich2646\picwgoal1500\pichgoal1500\jpegblip\bliptag-12486997"
'           {\*\blipuid ff4176ab418208a6a5c4f4f8a13d60d9}ffd8ffe000104a46494600010100000100010000ffe202284943435f50524f46494c450001010000021800000000043000006d6e74725247422058595a200000

						onepost.content = onepost.content + "<img src= "
						inimage% = 1
					CASE "/image"           ' finire... e togliere width/height
						PRINT #2, "}"; "}"; "}"; "}"; "}"
						onepost.content = onepost.content + "}" + "}" + "}" + "}" + "}"
						inimage% = 0
					CASE "item"
						initem% = 1
					CASE "/item"
						initem% = 0
					CASE "link"
						PRINT #2, "{\par\bkmkstart ";
						onepost.content = onepost.content + "{\bkmkstart "
						inlink% = 1
						' la GRAFFA andrebbe chiusa DOPO il link...
					CASE "/link"
						PRINT #2, "\par }{\bkmkend}"
						onepost.content = onepost.content + "\par }{\bkmkend "
						inlink% = 0
					CASE "pubDate"
						PRINT #2, "{\pard\fs22\li0\b"
						onepost.content = onepost.content + "{\b"
						indate% = 1
					CASE "/pubDate"
						indate% = 0
						PRINT #2, "}"
						onepost.content = onepost.content + "}"
					CASE "description"
						PRINT #2, "{\b\jc"            '
						onepost.content = onepost.content + "{\b"
					CASE "/description"
						PRINT #2, "}"
						onepost.content = onepost.content + "}"
						
					CASE "wp:comment_content"
						incomment% = 1
					CASE "/wp:comment_content"
						incomment% = 0
					CASE "wp:comment_date"
						incomment2% = 1
					CASE "/wp:comment_date"
						incomment2% = 0
					CASE "wp:comment_author"
						incomment3% = 1
					CASE "/wp:comment_author"
						incomment3% = 0

					CASE "em"
						checkBR
						PRINT #2, "{\i"
						onepost.content = onepost.content + "{\i"
					CASE "/em"
						checkBR
						PRINT #2, "}"
						onepost.content = onepost.content + "}"
					CASE "strong"
						checkBR
						PRINT #2, "{\b"
						onepost.content = onepost.content + "{\b"
					CASE "/strong"
						checkBR
						PRINT #2, "}"
						onepost.content = onepost.content + "}"
					CASE "strike"
						checkBR
						PRINT #2, "<strike>"
						onepost.content = onepost.content + "<strike>"
					CASE "/strike"
						checkBR
						PRINT #2, "</strike>"
						onepost.content = onepost.content + "</strike>"
'          CASE "span style=" + CHR$(34) + "text/decoration: underline" + CHR$(34)
					CASE "span style=" + CHR$(34) + "text-decoration: underline" + CHR$(34)
							checkBR
							PRINT #2, "{\ul"
							onepost.content = onepost.content + "{\ul"
					CASE "span"
							checkBR
							PRINT #2, "{"
							onepost.content = onepost.content + "{"
					CASE "/span"
						checkBR
						PRINT #2, "}"
						onepost.content = onepost.content + "}"
					CASE "ul"
						PRINT #2, "{\*\listtable{\list\listtemplateid68157441\listsimple";
						PRINT #2, "{\listlevel\levelnfc23\leveljc0\levelfollow0\levelstartat1\levelspace0\levelindent0";
						PRINT #2, "{\leveltext\'01\u-3913 ?;}";
						PRINT #2, "{\levelnumbers;}\f3\fbias0 \fi-360\li360\jclisttab\tx360 }";
						PRINT #2, "{\listname ;}";
						PRINT #2, "\listid161094266}";
						PRINT #2, "{\list\listtemplateid68157455\listsimple";
						PRINT #2, "{\listlevel\levelnfc0\leveljc0\levelfollow0\levelstartat1\levelspace0\levelindent0";
						PRINT #2, "{\leveltext\'02\'00.;}";
						PRINT #2, "{\levelnumbers\'01;}";
						PRINT #2, "\fi-360\li360\jclisttab\tx360 }";
						PRINT #2, "{\listname ;}";
						PRINT #2, "\listid1522279999}}"
						PRINT #2, "{\*\listoverridetable";
						PRINT #2, "{\listoverride\listid1522279999\listoverridecount0\ls1};"
						PRINT #2, "{\listoverride\listid161094266\listoverridecount0\ls2}"


						onepost.content = onepost.content + "<ul>"  ' finire
					CASE "/ul"
						PRINT #2, "}"
						onepost.content = onepost.content + "}"
					CASE "li"
						PRINT #2, "{\pntext\pard\plain\f3\fs20\cgrid \loch\af3\dbch\af0\hich\f3 \'b7\tab}\pard \fi-360\li360\widctlpar\jclisttab\tx360{\*\pn \pnlvlblt\ilvl0\ls2\pnrnot0\pnf3\pnstart1\pnindent360\pnhang{\pntxtb \'b7}}\ls2\adjustright {"
						onepost.content = onepost.content + "{\pntext\pard\plain\f3\fs20\cgrid \loch\af3\dbch\af0\hich\f3 \'b7\tab}\pard \fi-360\li360\widctlpar\jclisttab\tx360{\*\pn \pnlvlblt\ilvl0\ls2\pnrnot0\pnf3\pnstart1\pnindent360\pnhang{\pntxtb \'b7}}\ls2\adjustright {"
					CASE "/li"
						PRINT #2, "}"
						onepost.content = onepost.content + "}"
						

					CASE "dc:creator"
						inskip% = 1
					CASE "/dc:creator"
						inskip% = 0

				 
					CASE ELSE

						IF LEFT$(line$, 2) = "wp" THEN inwp% = 1
						IF LEFT$(line$, 3) = "/wp" THEN inwp% = 0
						IF LEFT$(line$, 4) = "guid" THEN inskip% = 1 ' il link interno
						IF LEFT$(line$, 5) = "/guid" THEN inskip% = 0
						IF intitle% AND LEFT$(line$, 8) = "![CDATA[" THEN
							IF first% = 0 THEN
								IF postdate <> oldpostdate THEN
									oldpostdate = postdate
									PRINT #2, "\page "; CHR$(10); "{\*\bkmkstart "; monthref$; "}\tab{\*\bkmkend "; monthref$; "}"
								ELSE
									PRINT #2, "\pard \qj\widctlpar\adjustright"
								END IF
							END IF
							IF first% = 1 THEN first% = 0

							onepost.title = ""     ' ...
							onepost.title = MID$(line$, 9, LEN(line$) - 10)   ' ]] finali...
							PRINT #2, "{\pard "; RTRIM$(onepost.title); "}"

							
							onepost.date = ""
							onepost.categories = ""
							onepost.link = ""
							onepost.author = ""
							onepost.content = ""
							onecomment.content = ""
							onecomment.date = ""
							onecomment.author = ""

						END IF
						IF LEFT$(line$, 6) = "a href" THEN
							checkBR
							PRINT #2, "{\field{\*\fldinst { HYPERLINK "; MID$(line$, 8)
							PRINT #2, "}}}{\fldrst {\cs15\ul\cf2 ";
							onepost.content = onepost.content + "{\field{\*\fldinst { HYPERLINK " + MID$(line$, 8) + "}}}{\fldrst {\cs15\ul\cf2 "
						END IF
						IF line$ = "/a" THEN
							checkBR
							PRINT #2, "}}"
							onepost.content = onepost.content + "}}"
						END IF
						IF LEFT$(line$, 9) = "div class" THEN   ' x youtube link, ev finire
							checkBR
							PRINT #2, "{\field{\*\fldinst { HYPERLINK " + line$ + " }{{\*\datafield "
							'PRINT #2, "00d0c9ea79f9bace118c8200aa004ba90b02000000170000001a000000680074007400700073003a002f002f007700770077002e00660061006300650062006f006f006b002e0063006f006d002f000000e0c9ea79f9bace118c8200aa004ba90b4c000000680074007400700073003a002f002f007700770077002e006600"
							'PRINT #2, "61006300650062006f006f006b002e0063006f006d002f000000795881f43b1d7f48af2c825dc485276300000000a5ab0000}}}{\fldrslt {\cs15\ul\cf2 "; line$;       ' "}}}"
							PRINT #2, "0000}}}{\fldrslt {\cs15\ul\cf2 "; line$       ' "}}}"

							onepost.content = onepost.content + "{\field{\*\fldinst { HYPERLINK " + line$   ' + "}}}"  ' finire..
						END IF
						IF LEFT$(line$, 9) = "div style" THEN   ' in pratica inutili
							checkBR
							PRINT #2, "{{{style"
						END IF
						IF LEFT$(line$, 4) = "/div" THEN
							checkBR
							PRINT #2, "}}}"
							onepost.content = onepost.content + "}}}"
						END IF
						IF LEFT$(line$, 8) = "category" THEN  ' tag/categorie
							checkBR
							a$ = MID$(line$, 38): a$ = LEFT$(a$, LEN(a$) - 1)
							IF a$ <> "uncategorized" THEN
								PRINT #2, "{\fs18\ql\widctlpar\adjustright Categoria "; a$; "; \par }"  ' controllare se non e' la prima e togliere la parola!
								onepost.categories = onepost.categories + ";" + a$
							END IF
						END IF
						IF LEFT$(line$, 9) = "/category" THEN
						 
						END IF

						IF LEFT$(line$, 8) = "ul class" THEN   ' diciamo patch per ora
							PRINT #2, "{"
						END IF

						IF incomment% THEN
							onecomment.content = MID$(line$, 9, LEN(line$) - 10)
							PRINT #2, "\li367 COMMENTO: "; RTRIM$(onecomment.content); "\pard "
						END IF
						IF incomment2% THEN
							onecomment.date = line$
							PRINT #2, "\li567 "; onecomment.date; "\pard "    'spariscono i trattini???
						END IF
						IF incomment3% THEN
							onecomment.author = MID$(line$, 9, LEN(line$) - 10)
							PRINT #2, "\li767 "; RTRIM$(onecomment.author); "\pard "
						END IF

				END SELECT
			END IF
			intag% = 0
			line$ = ""

		CASE ELSE
			IF a$ = CHR$(&HC3) THEN
				a$ = INPUT$(1, 1)
				SELECT CASE ASC(a$)
					CASE &HB2
						a$ = "o'"
					CASE &HB9
						a$ = "u'"
					CASE &HA0
						a$ = "a'"
					CASE &HA8
						a$ = "e'"
					CASE &HA9     ' perche'
						a$ = "e'"
					CASE &HAC
						a$ = "i'"
				END SELECT
			ELSE
				IF a$ = CHR$(&HE2) THEN
					a$ = INPUT$(1, 1)  ' 80h fisso
					a$ = INPUT$(1, 1)
					SELECT CASE ASC(a$)
						CASE &H99
							a$ = "'"
						CASE &H9C
							a$ = "?"
						CASE &H9D
							a$ = "?"
						CASE &HA6
							a$ = "?"
					END SELECT
				END IF
			END IF
			line$ = line$ + a$
			IF intag% = 0 THEN
				IF inwp% = 0 THEN

				END IF
			END IF

	END SELECT

	IF SEEK(1) / 1000 <> precpos& THEN
		LOCATE 3, 1
		PRINT USING "###.#%"; (SEEK(1) / tot&) * 100
		precpos& = SEEK(1)
	END IF

WEND

fine:
PRINT #2, "}"


CLOSE 2

CLOSE 1

PRINT : PRINT "Finito."

FUNCTION calcdate (a$)
	dYear% = VAL(MID$(a$, 13, 4))
	dDay% = VAL(MID$(a$, 6, 2))
'  PRINT dYear%, dDay%

	SELECT CASE MID$(a$, 9, 3)
		CASE "Jan"
			dMonth% = 1
		CASE "Feb"
			dMonth% = 2
		CASE "Mar"
			dMonth% = 3
		CASE "Apr"
			dMonth% = 4
		CASE "May"
			dMonth% = 5
		CASE "Jun"
			dMonth% = 6
		CASE "Jul"
			dMonth% = 7
		CASE "Aug"
			dMonth% = 8
		CASE "Sep"
			dMonth% = 9
		CASE "Oct"
			dMonth% = 10
		CASE "Nov"
			dMonth% = 11
		CASE "Dec"
			dMonth% = 12
	END SELECT

	calcdate = (dYear% + dMonth% - 1) / 12

END FUNCTION

SUB checkBR
	CLOSE 2
	OPEN of$ FOR INPUT AS 2
	SEEK 2, LOF(2) - 6
'  EXIT SUB
	a$ = INPUT$(4, 2)
	CLOSE 2
	OPEN of$ FOR APPEND AS 2
	IF a$ = "\par" THEN
		SEEK 2, LOF(2) - 6
		PRINT #2,
	ELSE
		SEEK 2, LOF(2)
	END IF
	
END SUB

FUNCTION getdate$

	getdate$ = "\yr" + MID$(DATE$, 7, 4) + "\mo" + MID$(DATE$, 4, 2) + "\dy" + MID$(DATE$, 1, 2) + "\hr" + MID$(TIME$, 1, 2) + "\min" + MID$(TIME$, 4, 2)

END FUNCTION

