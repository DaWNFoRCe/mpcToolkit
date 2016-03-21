//
//  MathUtil.cpp
//  edgeRuntime
//
//  Created by Abdelrahaman Aly on 04/02/14.
//
//

//Library Headers
#include <NTL/ZZ.h>
#include <NTL/ZZ_p.h>

//Custom Headers
#include "List.h"
#include "MathUtil.h"
using namespace NTL;
namespace Utilities
{
    //less significative bit on the left always most significative bit on the right always
    int * MathUtil::obtainBits(long number, int size)
    {
        int * bits= new int[size];
        for (int i=0; i<size; i++)
        {
            
            if (number&1)
            {

                bits[i]=1;
            }
            else
            {
                bits[i]=0;
            }
            
            number>>=1;

        }
        return bits;
    };
    
    //Calculates the lagrangian interpolation. Currently optimized for 3 players
    long MathUtil::lagrangianInterpolation( long * values, int size)
    {
        //TODO: build the lagrangian interpolation

        ZZ_p  result;
        result=2*conv<ZZ_p>(values[0]) -conv<ZZ_p>(values[1]);
        long resultLong =conv<long>(result);
        return  resultLong;
    };
    
    //calculates the complement 2 adding first aone and then decomposing into bits
    int* MathUtil::obtainComplementTwo(long  value, int size)
    {
        value=~value;
        value++;
        return MathUtil::obtainBits(value, size);
    };
    
    //obtains the additive of 2 mod values
    long MathUtil::additionMod(long a, long b)
    {
        ZZ_p ap= conv<ZZ_p>(a);
        ZZ_p bp= conv<ZZ_p>(b);
        return conv<long>(a+b);
    };
    
    //Obteins the multiplicative of 2 mod values
    long MathUtil::multiplyMod(long a, long b)
    {
        ZZ_p ap= conv<ZZ_p>(a);
        ZZ_p bp= conv<ZZ_p>(b);
        return conv<long>(a*b);
    };
    
    //An specification on generic multiplier of polynomials
    ZZ_pX MathUtil::multiplyLagrangePolynomials(vec_ZZ_p x_v, ZZ_p x_j)
    {

        ZZ_p x_d= x_j-x_v[0];
        ZZ_p a= 1/x_d;
        ZZ_pX p;
        p.SetLength(2);
        p[0]= (-1)*x_v[0]*a;
        p[1]= a;
     
        for (int k=1; k< x_v.length(); k++)
        {
            ZZ_p x_k =x_v[k];
            x_d= x_j-x_k;
            a= 1/x_d;
            
            ZZ_pX auxP;
            auxP.SetLength(2);
            auxP[0]= (-1)*x_k*a;
            auxP[1]=a;
            
            p=p*auxP;
        }
        return p;
    };
    
    //Interpolations:
    ZZ_pX MathUtil::multiplyLagrangePolynomials(int x_s, int x_k,int x_j)
    {
        vec_ZZ_p values;
        values.SetLength((x_k+1)-x_s -1);
        int j=0;
        for (int i = x_s; i<=x_k; i++)
        {
            if (i!= x_j) {
               values[j]= conv<ZZ_p>(i);
               j++;
            }

        }
        return MathUtil::multiplyLagrangePolynomials(values, conv<ZZ_p>(x_j));
    };
    
    int MathUtil::pow2roundup (int value)
    {
        int x=value;
        if (x < 0)
            return 0;
        --x;
        x |= x >> 1;
        x |= x >> 2;
        x |= x >> 4;
        x |= x >> 8;
        x |= x >> 16;
        return x+1;
    }
}
