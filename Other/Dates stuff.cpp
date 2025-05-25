vector<string> days_of_week_names_eng = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
vector<string> days_of_week_names_rus = {"Воскресенье", "Понедельник", "Вторник", "Среда", "Четверг", "Пятница", "Суббота"};
vector<string> months_names_eng = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
vector<string> months_names_rus = {"Январь", "Февраль", "Март", "Апрель", "Май", "Июнь", "Июль", "Август", "Сентябрь", "Октябрь", "Ноябрь", "Декабрь"};
vector<int> days_in_months = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

template<typename T>
bool is_leap_year(T y) {
    static_assert(is_signed_v<T>);
    return ((y % 400) == 0) || ((y % 4) == 0 && (y % 100));
}

template<typename T>
T date_to_int(T y, T m, T d) {
    static_assert(is_signed_v<T>);
    T jd = 1461 * (y + 4800 + (m - 14) / 12) / 4 +
             367 * (m - 2 - (m - 14) / 12 * 12) / 12 -
             3 * ((y + 4900 + (m - 14) / 12) / 100) / 4 +
             d - 32075;
    return jd;
}

template<typename T>
void int_to_date(T jd, T& y, T& m, T& d) {
    static_assert(is_signed_v<T>);
    T x, n, i, j;
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

template<typename T>
T day_of_week(T y, T m, T d) {
    static_assert(is_signed_v<T>);
    auto a = date_to_int(y, m, d);
    auto b = date_to_int(2000, 1, 2);
    return a < b ? (7 - (b - a) % 7) % 7 : (a - b) % 7;
}

template<typename T>
T days_in_year(T y) {
    static_assert(is_signed_v<T>);
    return 365 + is_leap_year(y);
}

template<typename T>
void day_of_year_to_day_and_month(T& d, T& m, T y) {
    static_assert(is_signed_v<T>);
    assert(1 <= d && d <= days_in_year(y));
    m = 0;
    for (--d; ; ++m) {
        T days_in_month = days_in_months[m] + (m == 1 && is_leap_year(y));
        if (d < days_in_month) break;
        d -= days_in_month;
    }
    ++m, ++d;
}
