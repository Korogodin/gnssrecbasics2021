#include "../header/head.h"

using namespace std;

int main()
{
    frame pack;
    otpravit(&pack);
    Ephemeris *Eph = (Ephemeris*)malloc(sizeof(Ephemeris));
    razobrat(Eph, &pack);
    time_t start, end;
    double t = 17+1 + 2*60*60;
    double* coord = new double[3];
    double* coord_matlab = new double[3];
    double max_del = 0;
    ofstream out;
    out.open("c_out.txt");
    ifstream in("matlab_out.txt");
    time(&start);
    for (int i = 0; i < 86400; i++)
    {
        Ocoord(t, coord, *Eph);
        t += 1;
        string coord_str1 = to_string(coord[0]);
        string coord_str2 = to_string(coord[1]);
        string coord_str3 = to_string(coord[2]);
        out << coord_str1 << " " << coord_str2 << " " << coord_str3 << endl;
        in >> coord_matlab[0] >> coord_matlab[1] >> coord_matlab[2];
        for (int j = 0; j < 3; j++)
        {
            if (abs(coord[j] - coord_matlab[j]) > max_del)
            {
                max_del = abs(coord[j] - coord_matlab[j]);
            }
        }
    }
    time(&end);
    in.close();
    delete[] coord;
    delete[] coord_matlab;
    coord = nullptr;
    coord_matlab = nullptr;
    double seconds = difftime(end, start);
    string seconds1 = to_string(seconds / 86400);
    cout << "\n\t\tОбщее время расчёта, с: " << seconds << endl;
    cout << "\n\t\tСреднее время расчёта, с: " << seconds1 << endl;
    string max_del1 = to_string(max_del);
    cout << "\n\t\tМаксимальная разность координат: " << max_del1 << endl;
    out.close();
    in.close();
    free(Eph);
}
