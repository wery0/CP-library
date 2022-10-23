vec<str> days_of_week_names = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
vec<str> months_names = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
vec<int> days_in_months = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

bool is_leap_year(int y) {
    return ((y % 400) == 0) || ((y % 4) == 0 && (y % 100));
}

int date_to_int(int y, int m, int d) {
    return 1461 * (y + 4800 + (m - 14) / 12) / 4 +
           367 * (m - 2 - (m - 14) / 12 * 12) / 12 -
           3 * ((y + 4900 + (m - 14) / 12) / 100) / 4 +
           d - 32075;
}

void int_to_date(int jd, int &y, int &m, int &d) {
    int x, n, i, j;
    x = jd + 68569;
    n = 4 * x / 146097;
    x -= (146097 * n + 3) / 4;
    i = (4000 * (x + 1)) / 1461001;
    x -= 1461 * i / 4 - 31;
    j = 80 * x / 2447;
    d = x - 2447 * j / 80;
    x = j / 11;
    m = j + 2 - 12 * x;
    y = 100 * (n - 49) + i + x;
}