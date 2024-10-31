#include<iostream>
#include<fstream>
using namespace std;

class OS
{
    char M[100][4], IR[4], R[4], buffer[40];
    int IC, SI;
    bool Toggle;

public:
    void INIT(), LOAD(), STARTEXECUTION(), EXECUTEUSERPROGRAM(), MOS(), READ(), WRITE(), TERMINATE();
    fstream Input, Output;
};

void OS::INIT()
{
    for (int i = 0; i < 100; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            M[i][j] = ' ';
        }
    }
    IR[4] = '\0';
    R[4] = '\0';
    Toggle = false;
}

void OS::LOAD()
{
    int x = 0;
    do
    {
        for (int i = 0; i < 40; i++)
        {
            buffer[i] = '\0';
        }

        Input.getline(buffer, 41);

        if (buffer[0] == '$' && buffer[1] == 'A' && buffer[2] == 'M' && buffer[3] == 'J')
        {
            INIT();
        }
        else if (buffer[0] == '$' && buffer[1] == 'D' && buffer[2] == 'T' && buffer[3] == 'A')
        {
            STARTEXECUTION();
        }
        else if (buffer[0] == '$' && buffer[1] == 'E' && buffer[2] == 'N' && buffer[3] == 'D')
        {
            x = 0;
            continue;
        }
        else
        {
            int b = 0;

            bool isInstruction = (buffer[0] == 'L' && buffer[1] == 'R') || (buffer[0] == 'S' && buffer[1] == 'R') ||
                                 (buffer[0] == 'C' && buffer[1] == 'R') || (buffer[0] == 'B' && buffer[1] == 'T') ||
                                 (buffer[0] == 'G' && buffer[1] == 'D') || (buffer[0] == 'P' && buffer[1] == 'D') ||
                                 (buffer[0] == 'H');

            if (isInstruction)
            {
                for (; x < 100; x++)
                {
                    for (int j = 0; j < 4; j++)
                    {
                        M[x][j] = buffer[b];
                        b++;
                    }

                    if (b == 40 || buffer[b] == ' ' || buffer[b] == '\n')
                    {
                        x++;
                        break;
                    }
                }

            }

        }

    } while (!Input.eof());

    for (int i = 0; i < 100; i++)
    {
        cout << "M[" << i << "]: ";
        for (int j = 0; j < 4; j++)
        {
            cout << M[i][j];
        }
        cout << endl;
    }

}




void OS::STARTEXECUTION()
{
    IC = 0;
    EXECUTEUSERPROGRAM();
}

void OS::EXECUTEUSERPROGRAM()
{
    while (true)
    {
        for (int i = 0; i < 4; i++)
        {
            IR[i] = M[IC][i];
        }
        IC++;

        if (IR[0] == 'L' && IR[1] == 'R')
        {
            int add = IR[2] - '0';
            add = (add * 10) + (IR[3] - '0');

            for (int i = 0; i < 4; i++)
                R[i] = M[add][i];
        }
        else if (IR[0] == 'S' && IR[1] == 'R')
        {
            int add = IR[2] - '0';
            add = (add * 10) + (IR[3] - '0');

            for (int i = 0; i < 4; i++)
                M[add][i] = R[i];
        }
        else if (IR[0] == 'C' && IR[1] == 'R')
        {
            int flag = 0;
            int add = IR[2] - '0';
            add = (add * 10) + (IR[3] - '0');

            for (int i = 0; i < 4; i++) {
                if (R[i] != M[add][i])
                    flag = 1;
            }

            if (flag == 1)
                Toggle = false;
            else
                Toggle = true;
        }
        else if (IR[0] == 'B' && IR[1] == 'T')
        {
            int add = IR[2] - '0';
            add = (add * 10) + (IR[3] - '0');
            if (Toggle == true)
                IC = add;
        }
        else if (IR[0] == 'G' && IR[1] == 'D')
        {
            SI = 1;
            READ();
        }
        else if (IR[0] == 'P' && IR[1] == 'D')
        {
            SI = 2;
            WRITE();
        }
        else if (IR[0] == 'H')
        {
            SI = 3;
            MOS();
            break;
        }
    }
}

void OS::MOS()
{
    switch (SI)
    {
    case 3:
        TERMINATE();
        break;
    }
}

void OS::READ() {
    for (int i = 0; i < 40; i++) {
        buffer[i] = '\0';
    }

    Input.getline(buffer, 41);

    int b = 0;
    int n = (IR[2] - '0') * 10 + (IR[3] - '0');

    for (int l = 0; l < 10; l++) {
        for (int j = 0; j < 4; j++) {
            M[n][j] = buffer[b];
            b++;
        }
        n++;
    }

    if (M[IR[2] - '0'][IR[3] - '0'] == '$' && M[IR[2] - '0'][IR[3] - '0' + 1] == 'E' && M[IR[2] - '0'][IR[3] - '0' + 2] == 'N' && M[IR[2] - '0'][IR[3] - '0' + 3] == 'D') {
        cout << "End of data encountered. Aborting." << endl;
    }

    SI = 0;
}

void OS::WRITE() {
    for (int i = 0; i < 40; i++) {
        buffer[i] = '\0';
    }

    int b = 0;
    int n = (IR[2] - '0') * 10 + (IR[3] - '0');

    for (int l = 0; l < 10; l++) {
        for (int j = 0; j < 4; j++) {
            buffer[b] = M[n][j];
            b++;
        }
        n++;
    }

    Output << buffer << endl;
    SI = 0;
}


void OS::TERMINATE()
{
    Output << "\n";
    Output << "\n";
    SI = 0;
}

int main()
{
    OS os;
    os.Input.open("job1ALL.txt", ios::in);
    os.Output.open("job1ALL_op.txt", ios::out);
    os.LOAD();
    os.Input.close();
    os.Output.close();
    return 0;
}
