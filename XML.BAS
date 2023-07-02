' converter for Wordpress Export XML files, so that the idiots over there won't cause any more damage blocking blogs for no reason
' G.Dar 2/7/2023

OPEN "wp.xml" FOR INPUT AS 1
OPEN "wpout.html" FOR OUTPUT AS 2

WHILE NOT EOF(1)
  a$ = INPUT$(1, 1)
  IF a$ = "" THEN GOTO fine
 
  SELECT CASE ASC(a$)
  CASE 60
    IF intag% = 0 THEN
      IF inwp% = 0 THEN
        IF line$ <> "" THEN
          IF ASC(line$) <> 10 THEN
            IF LEFT$(line$, 2) <> "]]" THEN
            PRINT #2, line$;
            ELSE
              PRINT #2, "<br>"
            END IF
          
            IF inimage% = 0 AND inlink% = 0 AND intitle% = 0 THEN PRINT #2, "<br>" ELSE PRINT #2,
          END IF
        END IF
      END IF
    END IF

    intag% = 1
    line$ = ""

  CASE 62
    IF intag% = 0 THEN
      IF inwp% = 0 THEN
        IF line$ <> "" THEN
          IF ASC(line$) <> 10 THEN
            IF LEFT$(line$, 2) <> "]]" THEN
              PRINT #2, line$;
            ELSE
              PRINT #2, "<br>"
            END IF
            IF inimage% = 0 AND inlink% = 0 AND intitle% = 0 THEN PRINT #2, "<br>" ELSE PRINT #2,
          END IF
        END IF
      END IF
    ELSE
      SELECT CASE line$
        CASE "title"
          PRINT #2, "<title>"
          intitle% = 1
        CASE "/title"
          PRINT #2, "</title>"
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
          PRINT #2, "<a name= "
          inlink% = 1
        CASE "/link"
          PRINT #2, "></a>"
          inlink% = 0
        CASE "pubDate"
          PRINT #2, "<b>"
        CASE "/pubDate"
          PRINT #2, "</b>"
        CASE "guid"     ' il link interno
          
        CASE "/guid"
          line$ = ""
          
        CASE "dc:creator"
       
        CASE "/dc:creator"
          line$ = ""

       
        CASE ELSE
          IF LEFT$(line$, 2) = "wp" THEN inwp% = 1
          IF LEFT$(line$, 3) = "/wp" THEN inwp% = 0
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

