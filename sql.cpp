
char sql_open_foreign_key[] = "PRAGMA foreign_keys = ON";

char sql_username[] = "default";
char sql_passwd[] = "default";

char sql_create_tb_student[] =
"\
    CREATE TABLE student( \
        Sno TEXT PRIMARY KEY NOT NULL, \
        Sname TEXT NOT NULL, \
        Ssex TEXT NOT NULL, \
        Sbd INT NOT NULL, \
        Sdept TEXT, \
        Scholarship TEXT NOT NULL\
    ) \
";

char sql_create_tb_course[] =
"\
    CREATE TABLE course ( \
        Cno TEXT PRIMARY KEY NOT NULL, \
        Cname TEXT NOT NULL, \
        Cpno TEXT, \
        Ccredit INT, \
        FOREIGN KEY (Cpno) REFERENCES Course(Cno) \
    ) \
";

char sql_create_tb_sc[] =
"\
        CREATE TABLE grade( \
            Sno TEXT NOT NULL, \
            Cno TEXT NOT NULL, \
            Grade INT, \
            PRIMARY key (Sno, Cno), \
            FOREIGN KEY (Sno) REFERENCES student(Sno), \
            FOREIGN KEY (Cno) REFERENCES course(Cno) \
        ) \
";

char sql_select_tb_student[] = "SELECT * FROM student";

char sql_insert_student[] = "INSERT INTO student VALUES(?,?,?,?,?,?)";

char sql_delete_student[] = "DELETE FROM student WHERE sno=?";

char sql_update_student[] = "UPDATE student SET %1 WHERE sno=%2";

char sql_select_tb_course[] = "SELECT * FROM course";

char sql_insert_course[] = "INSERT INTO course VALUES(?,?,?,?)";

char sql_delete_course[] = "DELETE FROM course WHERE cno=?";

char sql_update_course[] = "UPDATE course SET %1 WHERE cno=%2";

char sql_select_tb_grade[] = "SELECT * FROM grade";

char sql_insert_grade[] = "INSERT INTO grade VALUES(?,?,?)";

char sql_delete_grade[] = "DELETE FROM grade WHERE sno=? AND cno=?";

char sql_update_grade[] = "UPDATE grade SET grade=? WHERE sno=? AND cno=?";

char sql_stu_info1[] = "SELECT * FROM student WHERE sno=?";

char sql_stu_info2[] =
"\
    SELECT y.cno, cname, cpno, ccredit, grade \
    FROM student x, course y, grade z \
    WHERE x.sno=? AND z.sno=? AND y.cno=z.cno \
";
