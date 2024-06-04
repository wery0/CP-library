//Converts any integer in interval (-1e45, 1e45) to text
//Example: num_to_text(-123'000'700'914) = "minus one hundred twenty-three billion, seven hundred thousand, nine hundred fourteen"
template<typename T>
string num_to_text(T a) {
    if (a == 0) return "zero";
    if (a < 0) return "minus " + num_to_text(-a);
    static vector<string> m0 = {"zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine", "ten", "eleven", "twelve", "thirteen", "fourteen", "fifteen", "sixteen", "seventeen", "eighteen", "nineteen"};
    static vector<string> m1 = {"twenty", "thirty", "forty", "fifty", "sixty", "seventy", "eighty", "ninety"};
    static vector<string> orders = {"", "thousand", "million", "billion", "trillion", "quadrillion", "quintillion", "sextillion", "septillion", "octillion", "nonillion", "decillion", "undecilion", "duodecilion", "tredecillion"};
    auto less_1000 = [&](T a) -> string {
        string res = a > 99 ? m0[a / 100] + " hundred " : "";
        a %= 100;
        if (a) res += (a < 20 ? m0[a] : m1[a / 10 - 2] + (a % 10 ? "-" + m0[a % 10] : "")) + " ";
        res.pop_back();
        return res;
    };
    string res;
    for (size_t i = 0; a > 0; ++i, a /= 1000) {
        assert(i < orders.size() && "Add higher orders");
        if (a % 1000) res = less_1000(a % 1000) + " " + orders[i] + (res.empty() ? "" : ", ") + res;
    }
    return res;
}
