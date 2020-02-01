#include "biginteger.h"
#include <iostream>

int main()
{
    //std::ios_base::sync_with_stdio(0);
    //std::cin.tie(0);

    freopen("in.txt", "r", stdin);
    freopen("out_c.txt", "w", stdout);
    for(int i = 0; i < 1000; ++i){


        BigInteger a, b, c, d;
        std::cin >> a >> b;
        
        
        std::cout << (a / b) << "\n";
        //std::cin >> c >> d;
        //std::cout << c % d << "\n";

    }  
    //BigInteger a(0);
    //BigInteger b = BigInteger(5) - BigInteger(5);

    //std::cout << (a == 0) << " " << (b == 0) << "\n";


    return 0;    
}


        /*bool ok = true;
    for(int i = 0; i < 1000; ++i){
        int c, d;
        std::cin >> c >> d;
        BigInteger a(c), b(d);
        //std::cin >> a >> b;

        ++a %= b--;
        ++c %= d--;
        if(a != c){
            std::cout << a<< " " << c << "\n";
            ok = false;
        }
        
    }
        if(ok){
            std::cout << "OK\n";
        }*/
        /*BigInteger x28 = BigInteger(28);
        BigInteger x3 = BigInteger(3);


        std::cout << x28 % x3 << "\n";*/

        //std::cout << ((26 * 10) / 9) % 3 << "\n";
        /*BigInteger x26 = BigInteger(26);
        BigInteger x10 = BigInteger(10);
        BigInteger x9 = BigInteger(9);
        BigInteger x3 = BigInteger(3);

        BigInteger qwe = (((x26 * x10) / x9) % x3);

         std::cout << qwe << "\n"; */

    //}






/*        int a, b;
        std::cin >> a >> b;

        BigInteger A = BigInteger(a);
        BigInteger B = BigInteger(b);

        std::cout << A << " " << B << " " << (A - B) << "\n";
        printf("A < B : %d\n", (A < B));
        printf("A > B : %d\n", (A > B));
        printf("A == B : %d\n", (A == B));
        printf("A != B : %d\n", (A != B));
        printf("A <= B : %d\n", (A <= B));
        printf("A >= B : %d\n", (A >= B));*/
