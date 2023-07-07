' converter for Wordpress Export XML files, so that the idiots over there won't cause any more damage blocking blogs for no reason
' G.Dar 2/7/2023-5/7/2023

if$ = "wp.xml": of$ = "wpout.htm"
OPEN if$ FOR INPUT AS 1
OPEN of$ FOR OUTPUT AS 2

first% = 1
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
              ELSE
                PRINT #2, "<br>"
              END IF
            
              IF inimage% = 0 AND inlink% = 0 AND intitle% = 0 THEN PRINT #2, "<br>" ELSE PRINT #2,
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
            IF initem% = 0 THEN PRINT #2, "<title>"
            intitle% = 1
          CASE "/title"
            IF initem% = 0 THEN PRINT #2, "</title>"
            intitle% = 0
          CASE "image"
            PRINT #2, "<img src= "
            inimage% = 1
          CASE "/image"
            PRINT #2, ">"
            inimage% = 0
          CASE "item"
            initem% = 1
          CASE "/item"
            initem% = 0
          CASE "link"
            PRINT #2, "<a name= ";
            inlink% = 1
          CASE "/link"
            PRINT #2, "></a>"
            inlink% = 0
          CASE "pubDate"
            PRINT #2, "<b>"
          CASE "/pubDate"
            PRINT #2, "</b>"
          CASE "description"
            PRINT #2, "<h1>"
          CASE "/description"
            PRINT #2, "</h1>"
            
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
            PRINT #2, "<i>"
          CASE "/em"
            PRINT #2, "</i>"
          CASE "strong"
            PRINT #2, "<b>"
          CASE "/strong"
            PRINT #2, "</b>"
          CASE "strike"
            PRINT #2, "<strike>"
          CASE "/strike"
            PRINT #2, "</strike>"
          CASE "span style=" + CHR$(34) + "text/decoration: underline" + CHR$(34)
            PRINT #2, "<>"
          CASE "/span"
            PRINT #2, "</>"
          CASE "ul"
            PRINT #2, "<ul>"
          CASE "/ul"
            PRINT #2, "</ul>"
          CASE "li"
            PRINT #2, "<li>"
          CASE "/li"
            PRINT #2, "</li>"
            

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
              IF first% = 0 THEN PRINT #2, "<hr>"
              IF first% = 1 THEN first% = 0
              PRINT #2, "<h2>"; MID$(line$, 9, LEN(line$) - 10); "</h2>" ' ]] finali...
            END IF
            IF LEFT$(line$, 6) = "a href" THEN
              PRINT #2, "<"; line$; ">"
            END IF
            IF line$ = "/a" THEN
              PRINT #2, "<"; line$; ">"
            END IF
            IF incomment% THEN
              PRINT #2, "COMMENTO: "; MID$(line$, 9, LEN(line$) - 10); "<br>"; "<br>"
            END IF
            IF incomment2% THEN
              PRINT #2, "-"; line$; "<br>"    'spariscono i trattini???
            END IF
            IF incomment3% THEN
              PRINT #2, "--"; MID$(line$, 9, LEN(line$) - 10); "<br>"
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


WEND

fine:

CLOSE 2
CLOSE 1

