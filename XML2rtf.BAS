DECLARE SUB checkBR ()
DECLARE FUNCTION calcdate (a$)

' converter for Wordpress Export XML files to RTF, so that the idiots over there won't cause any more damage blocking blogs for no reason
' G.Dar 2/7/2023-17/7/2023

CLS
DIM SHARED of$

TYPE post
  title AS STRING * 128
  date AS STRING * 16
  author AS STRING * 32
  content AS STRING * 8000
  link AS STRING * 64
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
PRINT #2, "{\rtf"
PRINT #2, "<html=it>"

PRINT #2, "<! " + if$ + " converted by G.Dar converter on " + DATE$ + ">"

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

                onepost.content = onepost.content + line$
              ELSE
                PRINT #2, "<br>"
              END IF
            
              IF inimage% = 0 AND inlink% = 0 AND intitle% = 0 THEN PRINT #2, "<br>" ELSE PRINT #2,
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
                PRINT #2, "<br>"
              END IF
              IF inimage% = 0 AND inlink% = 0 AND intitle% = 0 THEN PRINT #2, "<br>" ELSE PRINT #2,
            END IF
          END IF
        ELSE

        END IF
      ELSE
        SELECT CASE line$
          CASE "title"
            IF initem% = 0 THEN PRINT #2, "<head>"; CHR$(10); "<title>"
            intitle% = 1
          CASE "/title"
            IF initem% = 0 THEN PRINT #2, "</title>"; CHR$(10); "</head>"; CHR$(10); "<body>"
            ' FINIRE esce anche con IMG SRC..
            intitle% = 0
          CASE "image"
            PRINT #2, "<img src= "
            onepost.content = onepost.content + "<img src= "
            inimage% = 1
          CASE "/image"           ' finire... e togliere width/height
            PRINT #2, ">"
            onepost.content = onepost.content + ">"
            inimage% = 0
          CASE "item"
            initem% = 1
          CASE "/item"
            initem% = 0
          CASE "link"
            PRINT #2, "<a name= ";
            onepost.content = onepost.content + "<a name= "
            inlink% = 1
          CASE "/link"
            PRINT #2, "></a>"
            onepost.content = onepost.content + "></a>"
            inlink% = 0
          CASE "pubDate"
            PRINT #2, "<b>"
            onepost.content = onepost.content + "<b>"
            indate% = 1
          CASE "/pubDate"
            indate% = 0
            PRINT #2, "</b>"
            onepost.content = onepost.content + "</b>"
          CASE "description"
            PRINT #2, "<h1>"
            onepost.content = onepost.content + "<h1>"
          CASE "/description"
            PRINT #2, "</h1>"
            onepost.content = onepost.content + "</h1>"
            
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
            PRINT #2, "<i>"
            onepost.content = onepost.content + "<i>"
          CASE "/em"
            checkBR
            PRINT #2, "</i>"
            onepost.content = onepost.content + "</i>"
          CASE "strong"
            checkBR
            PRINT #2, "<b>"
            onepost.content = onepost.content + "<b>"
          CASE "/strong"
            checkBR
            PRINT #2, "</b>"
            onepost.content = onepost.content + "</b>"
          CASE "strike"
            checkBR
            PRINT #2, "<strike>"
            onepost.content = onepost.content + "<strike>"
          CASE "/strike"
            checkBR
            PRINT #2, "</strike>"
            onepost.content = onepost.content + "</strike>"
          CASE "span style=" + CHR$(34) + "text/decoration: underline" + CHR$(34)
            checkBR
            PRINT #2, "<>"
            onepost.content = onepost.content + "<>"
          CASE "/span"
            checkBR
            PRINT #2, "</>"
            onepost.content = onepost.content + "</>"
          CASE "ul"
            PRINT #2, "<ul>"
            onepost.content = onepost.content + "<ul>"
          CASE "/ul"
            PRINT #2, "</ul>"
            onepost.content = onepost.content + "</ul>"
          CASE "li"
            PRINT #2, "<li>"
            onepost.content = onepost.content + "<li>"
          CASE "/li"
            PRINT #2, "</li>"
            onepost.content = onepost.content + "</li>"
            

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
                  PRINT #2, "<hr width=100%>"; CHR$(10); "<a name='DATESTAMP: "; monthref$; "'></a>"
                ELSE
                  PRINT #2, "<hr width=70%>"
                END IF
              END IF
              IF first% = 1 THEN first% = 0

              onepost.title = ""     ' ...
              onepost.title = MID$(line$, 9, LEN(line$) - 10)
              PRINT #2, "<h2>"; onepost.title; "</h2>" ' ]] finali...

              
              onepost.date = ""
              onepost.categories = ""
              onepost.link = ""
              onepost.author = ""
              onepost.content = ""

            END IF
            IF LEFT$(line$, 6) = "a href" THEN
              checkBR
              PRINT #2, "<"; line$; ">"

              onepost.content = onepost.content + "<" + line$ + ">"
            END IF
            IF line$ = "/a" THEN
              checkBR
              PRINT #2, "</a>"
              onepost.content = onepost.content + "</a>"
            END IF
            IF LEFT$(line$, 9) = "div class" THEN   ' x youtube link, ev finire
              checkBR
              PRINT #2, "<a href="; line$; ">"
              onepost.content = onepost.content + "<a href=" + line$ + ">"
            END IF
            IF LEFT$(line$, 4) = "/div" THEN
              checkBR
              PRINT #2, "</a>"
              onepost.content = onepost.content + "</a>"
            END IF
            IF LEFT$(line$, 8) = "category" THEN  ' tag/categorie
              checkBR
              a$ = MID$(line$, 38): a$ = LEFT$(a$, LEN(a$) - 1)
              IF a$ <> "uncategorized" THEN
                PRINT #2, "Categoria "; a$; "; <br>"  ' controllare se non e' la prima e togliere la parola!
                onepost.categories = onepost.categories + ";" + a$
              END IF
            END IF
            IF LEFT$(line$, 9) = "/category" THEN
             
            END IF

            IF incomment% THEN
              onecomment.content = MID$(line$, 9, LEN(line$) - 10)
              PRINT #2, "COMMENTO: "; onecomment.content; "<br>"; "<br>"
            END IF
            IF incomment2% THEN
              onecomment.date = line$
              PRINT #2, "-"; onecomment.date; "<br>"    'spariscono i trattini???
            END IF
            IF incomment3% THEN
              onecomment.author = MID$(line$, 9, LEN(line$) - 10)
              PRINT #2, "--"; onecomment.author; "<br>"
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
PRINT #2, "</body>"
PRINT #2, "</html>"

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
  SEEK 2, LOF(2) - 5
'  EXIT SUB
  a$ = INPUT$(2, 2)
  CLOSE 2
  OPEN of$ FOR APPEND AS 2
  IF a$ = "<b" THEN
    SEEK 2, LOF(2) - 5
    PRINT #2,
  ELSE
    SEEK 2, LOF(2)
  END IF
  
END SUB

