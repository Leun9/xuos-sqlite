
char sql_username[] = "default";
char sql_passwd[] = "default";

char sql_create_tb_student[] =
"\
    create table student( \
        Sno TEXT PRIMARY KEY NOT NULL, \
        Sname TEXT NOT NULL, \
        Ssex TEXT NOT NULL, \
        Sbd INT NOT NULL, \
        Sdept TEXT, \
        Scholarship INT NOT NULL\
    ) \
";

char sql_create_tb_course[] =
"\
    create table Course ( \
        Cno TEXT PRIMARY KEY, \
        Cname TEXT, \
        Cpno TEXT, \
        Ccredit INT, \
        FOREIGN KEY (Cpno) REFERENCES Course(Cno) \
    ) \
";

char sql_create_tb_sc[] =
"\
        create table SC( \
            Sno TEXT, \
            Cno TEXT, \
            Grade INT, \
            primary key (Sno, Cno), \
            FOREIGN KEY (Sno) REFERENCES Student(Sno), \
            FOREIGN KEY (Cno) REFERENCES Course(Cno) \
        ) \
";

char sql_select_tb_student[] = "SELECT * FROM student";

char sql_insert_student[] = "INSERT INTO student VALUES(?,?,?,?,?,?)";
