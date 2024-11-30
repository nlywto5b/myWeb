#include <iostream> // cin cout
#include <cmath>    // log sqrt
#include <climits>   // LONG_LONG_MAX
#include <cstring>   // string
#include <algorithm> // lower_bound
#include <windows.h> // windows api
using namespace std;

// const int MAX = 13'5818'7913;
const int MAX = sqrt(LONG_LONG_MAX / 5.0);

long long gcd(long long m, long long n)
{
    return n == 0 ? m : gcd(n, m % n);
}

int dig(long long n)
{
    return n == 0 ? 1 : (int)floor(log10(n) + 1);
}

void repeat_char(char c, int t)
{
    for (int i = 0; i < t; i++)
        cout << c;
}

void format(long long a, long long b, long long outer, long long inner, bool is2, string s)
{
    long long fm = 2 * a;
    if (inner == 0)
    { // 可直接计算
        long long fz = -1 * b + outer;
        if (is2)
            fz -= outer * 2;
        bool neg = false;
        if (fz * fm < 0)
            neg = true;
        fz = abs(fz);
        fm = abs(fm);
        long long div = gcd(abs(fz), abs(fm));
        fz /= div;
        fm /= div;
        if (fm == 1)
        {
            cout << s << " = ";
            if (neg)
                cout << "-";
            cout << fz << "\n\n";
            return;
        }
        int dig_fz = dig(fz);
        int dig_fm = dig(fm);
        int tot_dig = max(dig_fz, dig_fm);
        repeat_char(' ', s.length() + 3);
        if (neg)
            cout << "  ";
        repeat_char(' ', (tot_dig - dig_fz) / 2 + 1);
        cout << fz << endl;
        cout << s << " = ";
        if (neg)
            cout << "- ";
        repeat_char('-', tot_dig + 2);
        cout << endl;
        repeat_char(' ', s.length() + 3);
        if (neg)
            cout << "  ";
        repeat_char(' ', (tot_dig - dig_fm) / 2 + 1);
        cout << fm << "\n\n";
    }
    else
    {
        bool neg = false;
        long long div = gcd(gcd(abs(outer), abs(b)), fm);
        outer /= div;
        b /= div;
        fm /= div;
        if (is2)
            outer *= -1;
        if (outer < 0 && b >= 0)
        {
            outer *= -1;
            b *= -1;
            neg = !neg;
        }
        if (fm < 0)
        {
            fm *= -1;
            neg = !neg;
        }
        if (fm == 1)
        {
            if (neg)
            {
                outer *= -1;
                b *= -1;
                neg = !neg;
            }
            cout << s << " = ";
            if (neg)
                cout << "-";
            if (b != 0)
                cout << b * -1;
            if (outer > 0 && b != 0)
                cout << "+";
            else if (outer == -1)
                cout << "-";
            if (outer > 1 || outer < -1)
                cout << outer;
            cout << "√" << inner << "\n\n";
            return;
        }
        int dig_fz = dig(inner) + dig(abs(outer)) + dig(abs(b)) + 3;
        int dig_fm = dig(fm);
        int tot_dig = max(dig_fz, dig_fm);
        repeat_char(' ', s.length() + 3);
        if (neg)
            cout << "  ";
        repeat_char(' ', (tot_dig - dig_fz) / 2 + 1);
        if (b != 0)
            cout << b * -1;
        if (outer > 0 && b != 0)
            cout << "+";
        else if (outer == -1)
            cout << "-";
        if (outer > 1 || outer < -1)
            cout << outer;
        cout << "√" << inner << endl;
        cout << s << " = ";
        if (neg)
            cout << "- ";
        repeat_char('-', tot_dig + 2);
        cout << endl;
        repeat_char(' ', s.length() + 3);
        if (neg)
            cout << "  ";
        repeat_char(' ', (tot_dig - dig_fm) / 2 + 1);
        cout << fm << "\n\n";
    }
}

long long input(bool *is_ok, string *err)
{
    string s;
    cin >> s;
    long long res = 0;
    int len = s.length();
    bool is_neg = false;
    const string IV = "无效字符，请注意输入不得包含小数点、正号等其他字符\n";
    const string OR = "输入的数的绝对值不得超过" + to_string(MAX) + "\n";
    if (s[0] == '-')
    {
        is_neg = true;
    }
    if ((s[0] - '0' < 0 || s[0] - '0' > 9) && (!is_neg || s.length() == 1)) // invalid char
    {
        *is_ok = false;
        *err = IV;
        return -1;
    }
    else if (!is_neg)
    {
        res += s[0] - '0';
    }
    for (int i = 1; i < len; i++)
    {
        int digit = s[i] - '0';
        if (digit < 0 || digit > 9) // invalid char
        {
            *is_ok = false;
            *err = IV;
            return -1;
        }
        if (res > (MAX - digit) / 10) // out of range
        {
            *is_ok = false;
            *err = OR;
            return -2;
        }
        res *= 10;
        res += digit;
    }
    if (is_neg)
    {
        res *= -1;
    }
    return res;
}

const int N = 4e4 + 10;
const long long M = 3e6 + 10;

int prime[M];
bool v[M];
long long p[M];

// 埃氏筛
int Prime(int n)
{
    int m = 0;
    v[0] = v[1] = 1;
    for (int i = 2; i <= n; i++)
    {
        if (v[i] == 0)
            prime[++m] = i;
        for (int j = 1; j <= m && prime[j] <= n / i; j++)
        {
            v[i * prime[j]] = 1;
            if (i % prime[j] == 0)
                break;
        }
    }
    return m; // amount
}

void get_inner_outer(int m, long long *outer, long long *inner)
{
    long long x = *inner;
    for (int i = 1, cnt = 0; i <= m && p[i] <= x; i++, cnt = 0)
        while (x % prime[i] == 0)
        {
            cnt++;
            x /= prime[i];
            if (cnt % 2 == 0)
                *outer *= prime[i];
        }
    long long k = lower_bound(p + 1, p + M + 1, x) - p;
    if (k * k == x)
        *outer *= k;
    *inner = *inner / *outer / *outer;
}

const int WIDTH = 45;
const int HEIGHT = 40;
const int BUFFER_WIDTH = 45;
const int BUFFER_HEIGHT = 9000;

void init()
{
    HANDLE houtput = GetStdHandle(STD_OUTPUT_HANDLE);
    SMALL_RECT wrt = {0, 0, WIDTH - 1, HEIGHT - 1}; // -1
    SetConsoleWindowInfo(houtput, TRUE, &wrt);      // 窗口大小
    COORD coord = {BUFFER_WIDTH, BUFFER_HEIGHT};
    SetConsoleScreenBufferSize(houtput, coord); // 缓冲区大小
    HWND hwnd = GetConsoleWindow();
    LONG_PTR sty = GetWindowLongPtrA(hwnd, GWL_STYLE);
    sty = sty & ~WS_SIZEBOX & ~WS_MAXIMIZEBOX;
    SetWindowLongPtrA(hwnd, GWL_STYLE, sty); // 去除最大化按钮
    system("color 0F");                      // 颜色
    SetConsoleTitleA("一元二次方程求解");    // 标题
}

int main()
{
    init();
    cout << "一元二次方程求解\n";
    cout << "程序编写：牛力洋\n数据测试：李旭睿\n\n";
    cout << "请依次输入一元二次方程一般式ax^2 + bx + c = 0中的a、b、c，确保它们都是整数且绝对值小于" << MAX << "，然后按下Enter，程序将输出方程的根。\n";
    int m = Prime(N);
    for (int i = 1; i <= M; i++)
        p[i] = (long long)i * i;
    while (true)
    {
        repeat_char('-', 45);
        cout << "\n\n";

        long long a, b, c;
        bool is_ok = true;
        string err = "";
        cout << "a = ";
        a = input(&is_ok, &err);
        if (!is_ok)
        {
            cout << err << endl;
            continue;
        }
        cout << "b = ";
        b = input(&is_ok, &err);
        if (!is_ok)
        {
            cout << err << endl;
            continue;
        }
        if (a == 0 && b == 0)
        {
            cout << "a和b不能同时为0\n";
            continue;
        }
        cout << "c = ";
        c = input(&is_ok, &err);
        if (!is_ok)
        {
            cout << err << endl;
            continue;
        }
        if (a == 0)
        {
            c *= -1;
            bool neg = false;
            if (b * c < 0)
                neg = true;
            b = abs(b);
            c = abs(c);
            long long div = gcd(abs(b), abs(c));
            b /= div;
            c /= div;
            if (b == 1)
            {
                cout << "\nx = ";
                if (neg)
                    cout << "-";
                cout << c << "\n\n";
                continue;
            }
            int dig_fz = dig(b);
            int dig_fm = dig(c);
            int tot_dig = max(dig_fz, dig_fm);
            cout << "\n    ";
            if (neg)
                cout << "  ";
            repeat_char(' ', (tot_dig - dig_fz) / 2 + 1);
            cout << c << endl;
            cout << "x = ";
            if (neg)
                cout << "- ";
            repeat_char('-', tot_dig + 2);
            cout << endl;
            cout << "    ";
            if (neg)
                cout << "  ";
            repeat_char(' ', (tot_dig - dig_fm) / 2 + 1);
            cout << b << "\n\n";
            continue;
        }
        long long delta = b * b - 4 * a * c; // b^2 - 4ac
        long long outer = 1;
        long long inner = delta;
        cout << "Δ = " << delta << "\n\n";
        if (delta < 0)
        {
            cout << "Δ < 0, 原方程无实根\n\n";
            continue;
        }
        cout << "计算中\r";
        long long sq = (long long)sqrt(delta);
        if (sq * sq == delta)
        { // 检查完全平方
            outer = sq;
            inner = 0;
        }
        else
            get_inner_outer(m, &outer, &inner); // [1] Ning-H.[AHOI 2018初中组] 根式化简 题解.[EB/OL].https://www.cnblogs.com/Ning-H/p/11581108.html
        if (delta == 0)
            format(a, b, outer, inner, false, "x1 = x2");
        else
        {
            format(a, b, outer, inner, false, "x1");
            format(a, b, outer, inner, true, "x2");
        }
    }
    system("pause");
    return 0;
}
