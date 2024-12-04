vector<int> ZWJ = { -30, -128, -115};
vector<vector<int>> split(string s) {
    vector<vector<int>> o;
    for (int i = 0; i < s.size(); ) {
        int len = __builtin_clz(uint8_t(s[i]) ^ 255) - 24;
        assert(len >= 0);
        assert(len != 1);
        assert(len <= 4);
        if (len == 0) len = 1;
        vector<int> nc;
        for (int j = 0; j < len; ++j) {
            nc.push_back(s[i++]);
        }
        if (o.size() && o.back() == ZWJ) {
            o.pop_back();
            assert(o.size());
            o.back().insert(o.back().end(), ZWJ.begin(), ZWJ.end());
            o.back().insert(o.back().end(), nc.begin(), nc.end());
        } else {
            o.push_back(nc);
        }
    }
    return o;
}

class chr {
    vector<int> m;

public:
    auto begin() {return m.begin();}
    auto begin() const {return m.begin();}
    auto end() {return m.end();}
    auto end() const {return m.end();}

    chr() = default;
    chr(vector<int> m): m(m) {}
    chr(string s) {
        vector<vector<int>> o = split(s);
        if (o.size() == 1) *this = o[0];
    }
    chr(char c) {
        *this = chr(string(1, c));
    }

    bool operator==(const chr& rhs) const {return m == rhs.m;}
    bool operator==(const char& rhs) const {return m == chr(rhs).m;}
    bool operator!=(const chr& rhs) const {return m != rhs.m;}
    bool operator<(const chr& rhs) const {return m < rhs.m;}
    bool operator>(const chr& rhs) const {return m > rhs.m;}

    size_t size() const {return m.size();}

    friend istream& operator>>(istream& in, chr& rhs) {
        char c; cin >> c;
        int len = __builtin_clz(uint8_t(c) ^ 255) - 24;
        string s(1, c);
        for (int i = 1; i < len; ++i) cin >> c, s += c;
        rhs = chr(s);
        return in;
    }
    friend ostream& operator<<(ostream& out, const chr& rhs) {string s; for (auto i : rhs.m) s += i; out << s; return out;}
};

string to_string(const chr& m) {
    string s;
    for (auto c : m) s += c;
    return s;
}

template<typename T>
vector<T> int_to_unicode_bytes(int x) {
    int len = x < 128 ? 1 : x < 0x0800 ? 2 : x < 0x010000 ? 3 : x < 0x110000 ? 4 : 5;
    assert(len < 5);
    vector<T> res(len, 0b10000000);
    res[0] = len == 1 ? x : len == 2 ? 0b11000000 : len == 3 ? 0b11100000 : 0b11110000;
    for (int i = len - 1; i >= 0; --i) {
        int frb = i ? 6 : len == 1 ? 7 : 7 - len;
        for (int j = 0; j < frb; ++j, x >>= 1) {
            res[i] |= (x & 1) << j;
        }
    }
    return res;
}

class utf8string {
    vector<chr> m;

public:
    utf8string() = default;
    utf8string(const string& s) {for (auto c : split(s)) m.push_back(chr(c));}
    utf8string(const char c) {*this = utf8string(string(1, c));}
    utf8string(const char* chars) {
        string s;
        for (auto i = chars; *i; ++i) s += *i;
        *this = utf8string(s);
    }

    size_t size() const {return m.size();}
    void clear() {m.clear();}
    chr operator[](size_t i) {assert(i < size()); return m[i];}
    void push_back(const chr& c) {m.push_back(c);}
    void pop_back() {m.pop_back();}
    chr& back() {return m.back();}
    template<typename I> void erase(I begin, I end) {m.erase(begin, end);}

    utf8string& operator+=(const utf8string& rhs) {m.insert(m.end(), rhs.m.begin(), rhs.m.end()); return *this;}
    utf8string& operator+=(const chr& rhs) {m.push_back(rhs); return *this;}
    utf8string operator+(const utf8string& rhs) {return utf8string(*this) += rhs;}

    bool operator==(const utf8string& rhs) const {return m == rhs.m;}
    bool operator==(const string& rhs) const {return m == utf8string(rhs).m;}
    bool operator!=(const utf8string& rhs) const {return m != rhs.m;}
    bool operator<(const utf8string& rhs) const {return m < rhs.m;}
    bool operator>(const utf8string& rhs) const {return m > rhs.m;}

    auto begin() {return m.begin();}
    auto begin() const {return m.begin();}
    auto end() {return m.end();}
    auto end() const {return m.end();}

    utf8string substr(size_t l, size_t len) {
        assert(l + len <= size());
        utf8string res = *this;
        res.erase(res.begin() + l + len, res.end());
        res.erase(res.begin(), res.begin() + l);
        return res;
    }

    friend istream& operator>>(istream& in, utf8string& rhs) {string s; in >> s; rhs = utf8string(s); return in;}
    friend ostream& operator<<(ostream& out, const utf8string& rhs) {for (const auto& c : rhs) out << c; return out;}
};
using utfstr = utf8string;

utf8string lc("abcdefghijklmnopqrstuvwxyzµàáâãäåæçèéêëìíîïðñòóôõöøùúûüýþÿāăąćĉċčďđēĕėęěĝğġģĥħĩīĭįĳĵķĺļľŀłńņňŋōŏőœŕŗřśŝşšţťŧũūŭůűųŵŷźżžƀƃƅƈƌƒƕƙƚƞơƣƥƨƭưƴƶƹƽƿǆǉǌǎǐǒǔǖǘǚǜǝǟǡǣǥǧǩǫǭǯǳǵǹǻǽǿȁȃȅȇȉȋȍȏȑȓȕȗșțȝȟȣȥȧȩȫȭȯȱȳȼȿɀɂɇɉɋɍɏɐɑɒɓɔɖɗəɛɜɠɡɣɥɦɨɩɪɫɬɯɱɲɵɽʀʂʃʇʈʉʊʋʌʒʝʞͅͱͳͷͻͼͽάέήίαβγδεζηθκλνξοπρςτυφχψωϊϋόύώϗϙϛϝϟϡϣϥϧϩϫϭϯϲϳϸϻабвгдежзийклмнопрстуфхцчшщъыьэюяѐёђѓєѕіїјљњћќѝўџѡѣѥѧѩѫѭѯѱѳѵѷѹѻѽѿҁҋҍҏґғҕҗҙқҝҟҡңҥҧҩҫҭүұҳҵҷҹһҽҿӂӄӆӈӊӌӎӏӑӓӕӗәӛӝӟӡӣӥӧөӫӭӯӱӳӵӷӹӻӽӿԁԃԅԇԉԋԍԏԑԓԕԗԙԛԝԟԡԣԥԧԩԫԭԯաբգդեզէըթժիլխծկհձղճմյնշոչպջռսվտրցւփքօֆაბგდევზთიკლმნოპჟრსტუფქღყშჩცძწჭხჯჰჱჲჳჴჵჶჷჸჹჺჽჾჿᏸᏹᏺᏻᏼᏽᲈᵹᵽᶎḁḃḅḇḉḋḍḏḑḓḕḗḙḛḝḟḡḣḥḧḩḫḭḯḱḳḵḷḹḻḽḿṁṃṅṇṉṋṍṏṑṓṕṗṙṛṝṟṡṣṥṧṩṫṭṯṱṳṵṷṹṻṽṿẁẃẅẇẉẋẍẏẑẓẕạảấầẩẫậắằẳẵặẹẻẽếềểễệỉịọỏốồổỗộớờởỡợụủứừửữựỳỵỷỹỻỽỿἀἁἂἃἄἅἆἇἐἑἒἓἔἕἠἡἢἣἤἥἦἧἰἱἲἳἴἵἶἷὀὁὂὃὄὅὑὓὕὗὠὡὢὣὤὥὦὧὰάὲέὴήὶίὸόὺύὼώᾰᾱῐῑῠῡῥⅎⅰⅱⅲⅳⅴⅵⅶⅷⅸⅹⅺⅻⅼⅽⅾⅿↄⓐⓑⓒⓓⓔⓕⓖⓗⓘⓙⓚⓛⓜⓝⓞⓟⓠⓡⓢⓣⓤⓥⓦⓧⓨⓩⰰⰱⰲⰳⰴⰵⰶⰷⰸⰹⰺⰻⰼⰽⰾⰿⱀⱁⱂⱃⱄⱅⱆⱇⱈⱉⱊⱋⱌⱍⱎⱏⱐⱑⱒⱓⱔⱕⱖⱗⱘⱙⱚⱛⱜⱝⱞⱟⱡⱥⱦⱨⱪⱬⱳⱶⲁⲃⲅⲇⲉⲋⲍⲏⲑⲓⲕⲗⲙⲛⲝⲟⲡⲣⲥⲧⲩⲫⲭⲯⲱⲳⲵⲷⲹⲻⲽⲿⳁⳃⳅⳇⳉⳋⳍⳏⳑⳓⳕⳗⳙⳛⳝⳟⳡⳣⳬⳮⳳⴀⴁⴂⴃⴄⴅⴆⴇⴈⴉⴊⴋⴌⴍⴎⴏⴐⴑⴒⴓⴔⴕⴖⴗⴘⴙⴚⴛⴜⴝⴞⴟⴠⴡⴢⴣⴤⴥⴧⴭꙁꙃꙅꙇꙉꙍꙏꙑꙓꙕꙗꙙꙛꙝꙟꙡꙣꙥꙧꙩꙫꙭꚁꚃꚅꚇꚉꚋꚍꚏꚑꚓꚕꚗꚙꚛꜣꜥꜧꜩꜫꜭꜯꜳꜵꜷꜹꜻꜽꜿꝁꝃꝅꝇꝉꝋꝍꝏꝑꝓꝕꝗꝙꝛꝝꝟꝡꝣꝥꝧꝩꝫꝭꝯꝺꝼꝿꞁꞃꞅꞇꞌꞑꞓꞔꞗꞙꞛꞝꞟꞡꞣꞥꞧꞩꞵꞷꞹꞻꞽꞿꟁꟃꟈꟊꟑꟗꟙꟶꭓꭰꭱꭲꭳꭴꭵꭶꭷꭸꭹꭺꭻꭼꭽꭾꭿꮀꮁꮂꮃꮄꮅꮆꮇꮈꮉꮊꮋꮌꮍꮎꮏꮐꮑꮒꮓꮔꮕꮖꮗꮘꮙꮚꮛꮜꮝꮞꮟꮠꮡꮢꮣꮤꮥꮦꮧꮨꮩꮪꮫꮬꮭꮮꮯꮰꮱꮲꮳꮴꮵꮶꮷꮸꮹꮺꮻꮼꮽꮾꮿａｂｃｄｅｆｇｈｉｊｋｌｍｎｏｐｑｒｓｔｕｖｗｘｙｚ𐐨𐐩𐐪𐐫𐐬𐐭𐐮𐐯𐐰𐐱𐐲𐐳𐐴𐐵𐐶𐐷𐐸𐐹𐐺𐐻𐐼𐐽𐐾𐐿𐑀𐑁𐑂𐑃𐑄𐑅𐑆𐑇𐑈𐑉𐑊𐑋𐑌𐑍𐑎𐑏𐓘𐓙𐓚𐓛𐓜𐓝𐓞𐓟𐓠𐓡𐓢𐓣𐓤𐓥𐓦𐓧𐓨𐓩𐓪𐓫𐓬𐓭𐓮𐓯𐓰𐓱𐓲𐓳𐓴𐓵𐓶𐓷𐓸𐓹𐓺𐓻𐖗𐖘𐖙𐖚𐖛𐖜𐖝𐖞𐖟𐖠𐖡𐖣𐖤𐖥𐖦𐖧𐖨𐖩𐖪𐖫𐖬𐖭𐖮𐖯𐖰𐖱𐖳𐖴𐖵𐖶𐖷𐖸𐖹𐖻𐖼𐳀𐳁𐳂𐳃𐳄𐳅𐳆𐳇𐳈𐳉𐳊𐳋𐳌𐳍𐳎𐳏𐳐𐳑𐳒𐳓𐳔𐳕𐳖𐳗𐳘𐳙𐳚𐳛𐳜𐳝𐳞𐳟𐳠𐳡𐳢𐳣𐳤𐳥𐳦𐳧𐳨𐳩𐳪𐳫𐳬𐳭𐳮𐳯𐳰𐳱𐳲𑣀𑣁𑣂𑣃𑣄𑣅𑣆𑣇𑣈𑣉𑣊𑣋𑣌𑣍𑣎𑣏𑣐𑣑𑣒𑣓𑣔𑣕𑣖𑣗𑣘𑣙𑣚𑣛𑣜𑣝𑣞𑣟𖹠𖹡𖹢𖹣𖹤𖹥𖹦𖹧𖹨𖹩𖹪𖹫𖹬𖹭𖹮𖹯𖹰𖹱𖹲𖹳𖹴𖹵𖹶𖹷𖹸𖹹𖹺𖹻𖹼𖹽𖹾𖹿𞤢𞤣𞤤𞤥𞤦𞤧𞤨𞤩𞤪𞤫𞤬𞤭𞤮𞤯𞤰𞤱𞤲𞤳𞤴𞤵𞤶𞤷𞤸𞤹𞤺𞤻𞤼𞤽𞤾𞤿𞥀𞥁𞥂𞥃");
utf8string uc("ABCDEFGHIJKLMNOPQRSTUVWXYZΜÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏÐÑÒÓÔÕÖØÙÚÛÜÝÞŸĀĂĄĆĈĊČĎĐĒĔĖĘĚĜĞĠĢĤĦĨĪĬĮĲĴĶĹĻĽĿŁŃŅŇŊŌŎŐŒŔŖŘŚŜŞŠŢŤŦŨŪŬŮŰŲŴŶŹŻŽɃƂƄƇƋƑǶƘȽȠƠƢƤƧƬƯƳƵƸƼǷǄǇǊǍǏǑǓǕǗǙǛƎǞǠǢǤǦǨǪǬǮǱǴǸǺǼǾȀȂȄȆȈȊȌȎȐȒȔȖȘȚȜȞȢȤȦȨȪȬȮȰȲȻⱾⱿɁɆɈɊɌɎⱯⱭⱰƁƆƉƊƏƐꞫƓꞬƔꞍꞪƗƖꞮⱢꞭƜⱮƝƟⱤƦꟅƩꞱƮɄƱƲɅƷꞲꞰΙͰͲͶϽϾϿΆΈΉΊΑΒΓΔΕΖΗΘΚΛΝΞΟΠΡΣΤΥΦΧΨΩΪΫΌΎΏϏϘϚϜϞϠϢϤϦϨϪϬϮϹͿϷϺАБВГДЕЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯЀЁЂЃЄЅІЇЈЉЊЋЌЍЎЏѠѢѤѦѨѪѬѮѰѲѴѶѸѺѼѾҀҊҌҎҐҒҔҖҘҚҜҞҠҢҤҦҨҪҬҮҰҲҴҶҸҺҼҾӁӃӅӇӉӋӍӀӐӒӔӖӘӚӜӞӠӢӤӦӨӪӬӮӰӲӴӶӸӺӼӾԀԂԄԆԈԊԌԎԐԒԔԖԘԚԜԞԠԢԤԦԨԪԬԮԱԲԳԴԵԶԷԸԹԺԻԼԽԾԿՀՁՂՃՄՅՆՇՈՉՊՋՌՍՎՏՐՑՒՓՔՕՖᲐᲑᲒᲓᲔᲕᲖᲗᲘᲙᲚᲛᲜᲝᲞᲟᲠᲡᲢᲣᲤᲥᲦᲧᲨᲩᲪᲫᲬᲭᲮᲯᲰᲱᲲᲳᲴᲵᲶᲷᲸᲹᲺᲽᲾᲿᏰᏱᏲᏳᏴᏵꙊꝽⱣꟆḀḂḄḆḈḊḌḎḐḒḔḖḘḚḜḞḠḢḤḦḨḪḬḮḰḲḴḶḸḺḼḾṀṂṄṆṈṊṌṎṐṒṔṖṘṚṜṞṠṢṤṦṨṪṬṮṰṲṴṶṸṺṼṾẀẂẄẆẈẊẌẎẐẒẔẠẢẤẦẨẪẬẮẰẲẴẶẸẺẼẾỀỂỄỆỈỊỌỎỐỒỔỖỘỚỜỞỠỢỤỦỨỪỬỮỰỲỴỶỸỺỼỾἈἉἊἋἌἍἎἏἘἙἚἛἜἝἨἩἪἫἬἭἮἯἸἹἺἻἼἽἾἿὈὉὊὋὌὍὙὛὝὟὨὩὪὫὬὭὮὯᾺΆῈΈῊΉῚΊῸΌῪΎῺΏᾸᾹῘῙῨῩῬℲⅠⅡⅢⅣⅤⅥⅦⅧⅨⅩⅪⅫⅬⅭⅮⅯↃⒶⒷⒸⒹⒺⒻⒼⒽⒾⒿⓀⓁⓂⓃⓄⓅⓆⓇⓈⓉⓊⓋⓌⓍⓎⓏⰀⰁⰂⰃⰄⰅⰆⰇⰈⰉⰊⰋⰌⰍⰎⰏⰐⰑⰒⰓⰔⰕⰖⰗⰘⰙⰚⰛⰜⰝⰞⰟⰠⰡⰢⰣⰤⰥⰦⰧⰨⰩⰪⰫⰬⰭⰮⰯⱠȺȾⱧⱩⱫⱲⱵⲀⲂⲄⲆⲈⲊⲌⲎⲐⲒⲔⲖⲘⲚⲜⲞⲠⲢⲤⲦⲨⲪⲬⲮⲰⲲⲴⲶⲸⲺⲼⲾⳀⳂⳄⳆⳈⳊⳌⳎⳐⳒⳔⳖⳘⳚⳜⳞⳠⳢⳫⳭⳲႠႡႢႣႤႥႦႧႨႩႪႫႬႭႮႯႰႱႲႳႴႵႶႷႸႹႺႻႼႽႾႿჀჁჂჃჄჅჇჍꙀꙂꙄꙆꙈꙌꙎꙐꙒꙔꙖꙘꙚꙜꙞꙠꙢꙤꙦꙨꙪꙬꚀꚂꚄꚆꚈꚊꚌꚎꚐꚒꚔꚖꚘꚚꜢꜤꜦꜨꜪꜬꜮꜲꜴꜶꜸꜺꜼꜾꝀꝂꝄꝆꝈꝊꝌꝎꝐꝒꝔꝖꝘꝚꝜꝞꝠꝢꝤꝦꝨꝪꝬꝮꝹꝻꝾꞀꞂꞄꞆꞋꞐꞒꟄꞖꞘꞚꞜꞞꞠꞢꞤꞦꞨꞴꞶꞸꞺꞼꞾꟀꟂꟇꟉꟐꟖꟘꟵꞳᎠᎡᎢᎣᎤᎥᎦᎧᎨᎩᎪᎫᎬᎭᎮᎯᎰᎱᎲᎳᎴᎵᎶᎷᎸᎹᎺᎻᎼᎽᎾᎿᏀᏁᏂᏃᏄᏅᏆᏇᏈᏉᏊᏋᏌᏍᏎᏏᏐᏑᏒᏓᏔᏕᏖᏗᏘᏙᏚᏛᏜᏝᏞᏟᏠᏡᏢᏣᏤᏥᏦᏧᏨᏩᏪᏫᏬᏭᏮᏯＡＢＣＤＥＦＧＨＩＪＫＬＭＮＯＰＱＲＳＴＵＶＷＸＹＺ𐐀𐐁𐐂𐐃𐐄𐐅𐐆𐐇𐐈𐐉𐐊𐐋𐐌𐐍𐐎𐐏𐐐𐐑𐐒𐐓𐐔𐐕𐐖𐐗𐐘𐐙𐐚𐐛𐐜𐐝𐐞𐐟𐐠𐐡𐐢𐐣𐐤𐐥𐐦𐐧𐒰𐒱𐒲𐒳𐒴𐒵𐒶𐒷𐒸𐒹𐒺𐒻𐒼𐒽𐒾𐒿𐓀𐓁𐓂𐓃𐓄𐓅𐓆𐓇𐓈𐓉𐓊𐓋𐓌𐓍𐓎𐓏𐓐𐓑𐓒𐓓𐕰𐕱𐕲𐕳𐕴𐕵𐕶𐕷𐕸𐕹𐕺𐕼𐕽𐕾𐕿𐖀𐖁𐖂𐖃𐖄𐖅𐖆𐖇𐖈𐖉𐖊𐖌𐖍𐖎𐖏𐖐𐖑𐖒𐖔𐖕𐲀𐲁𐲂𐲃𐲄𐲅𐲆𐲇𐲈𐲉𐲊𐲋𐲌𐲍𐲎𐲏𐲐𐲑𐲒𐲓𐲔𐲕𐲖𐲗𐲘𐲙𐲚𐲛𐲜𐲝𐲞𐲟𐲠𐲡𐲢𐲣𐲤𐲥𐲦𐲧𐲨𐲩𐲪𐲫𐲬𐲭𐲮𐲯𐲰𐲱𐲲𑢠𑢡𑢢𑢣𑢤𑢥𑢦𑢧𑢨𑢩𑢪𑢫𑢬𑢭𑢮𑢯𑢰𑢱𑢲𑢳𑢴𑢵𑢶𑢷𑢸𑢹𑢺𑢻𑢼𑢽𑢾𑢿𖹀𖹁𖹂𖹃𖹄𖹅𖹆𖹇𖹈𖹉𖹊𖹋𖹌𖹍𖹎𖹏𖹐𖹑𖹒𖹓𖹔𖹕𖹖𖹗𖹘𖹙𖹚𖹛𖹜𖹝𖹞𖹟𞤀𞤁𞤂𞤃𞤄𞤅𞤆𞤇𞤈𞤉𞤊𞤋𞤌𞤍𞤎𞤏𞤐𞤑𞤒𞤓𞤔𞤕𞤖𞤗𞤘𞤙𞤚𞤛𞤜𞤝𞤞𞤟𞤠𞤡");

map<chr, chr> lctuc = []() {
    assert(lc.size() == uc.size());
    map<chr, chr> res;
    for (int i = 0; i < lc.size(); ++i) {
        res[lc[i]] = uc[i];
    }
    return res;
}();

map<chr, chr> uctlc = []() {
    assert(lc.size() == uc.size());
    map<chr, chr> res;
    for (int i = 0; i < lc.size(); ++i) {
        res[uc[i]] = lc[i];
    }
    return res;
}();

chr toupper(chr c) {return lctuc.count(c) ? to_string(lctuc.at(c)) : to_string(c);}
chr tolower(chr c) {return uctlc.count(c) ? to_string(uctlc.at(c)) : to_string(c);}
string to_string(utf8string s) {string res; for (auto c : s) res += to_string(c); return res;}
int stoi(utf8string s) {return stoi(to_string(s));}
long long stoll(utf8string s) {return stoll(to_string(s));}
void getline(istream& in, utfstr& rhs) {string s; getline(in, s); rhs = utfstr(s);}
