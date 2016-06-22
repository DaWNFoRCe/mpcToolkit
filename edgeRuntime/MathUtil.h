//
//  MathUtil.h
//  edgeRuntime
//
//  Created by Abdelrahaman Aly on 04/02/14.
//
//

#ifndef edgeRuntime_MathUtil_h
#define edgeRuntime_MathUtil_h
//Library Headers
#include <NTL/ZZ_pX.h>
#include <NTL/ZZ_p.h>

//Custom Headers
#include "StandardShare.h"
#include "List.h"

using namespace NTL;
namespace Utilities
{
    /**
     * @class MathUtil
     * @ingroup Utilities
     * @brief
     * Utility class for Mathematic operations.
     * @details Static class. Do not instantiate.
     * @todo
     <ul>
     <li> Create a private constructor.
     <li> Implement Exception Control.
    <li>  Change the class methods to make them more modular, real object oriented, like the lagrange functionality
     <\ul>
     * @author Abdelrahaman Aly
     * @date  2014/02/24
     * @version 0.0.1.15
     */
    class MathUtil
    {
    private:
    public:
        
        /**
         * @brief Decompose a number into bits
         * @details In case the number is not big enough it cipmpletes it with 0 unitl it fullfils the size parameter. The process is C coupled, with Bit deplacements instead of sucesive divisions.
         * @note The less significative bit on the left always most significative bit on the right always Example:
            <p><b>
                1011 = 13
            </b></p>
         * @param number value to be decompose into bits
         * @param bit size of the decomposition
         * @return size An int vector of size <it>size</it>
         * @exception NA
         * @todo
         <ul>
         <li>Build Exceptions for non convertible data
         </ul>
         */
        static int * obtainBits(long number, int size);

        /**
         * @brief Obtains the langrangian interpolation os a given size.
         * @details It calculcates the lagrangian based on thei input data. The process now is optimized for 3 players.
         * @param values vector of values to be interpolated
         * @param size size of the interpolation
         * @return bits an int vector of size <it>size<\it>
         * @exception NA
         * @todo
         <ul>
         <li>Build Exceptions for non interpolable data
         </ul>
         */
        static long lagrangianInterpolation( long * values, int size);
        
        /**
         * @brief Obtains the complement 2 of a vector of bits.
         * @param value decimal value, base of the calculus
         * @param size size of the vector of bits
         * @return bits an int vector of size "size" with bits on complement 2
         * @exception NA
         * @todo
         <ul>
         <li> Validate in case the vector is not conviertible
         <li>Build Exceptions for non interpolable data
         </ul>
         */
        static int * obtainComplementTwo(long  value, int size);

        /**
         * @brief Performs an addition in a predefined system mod.
         * @details calculations are done using NTL libraries.
         * @note all values are also in mod the operation is going to be performed. The mod on the NTL libraries has to be defined prior these methods are used.
         * @param a value to be added.
         * @param b value to be added.
         * @return c c=a+b mod m (m is given on the configuration)
         * @exception NA
         * @todo
         <ul>
         <li>Build Exceptions for non mod data
         </ul>
         */
        static long additionMod(long a, long b);
        
        /**
         * @brief Performs an multiplication in a predefined system mod.
         * @details calculations are done using NTL libraries.
         * @note all values are also in mod the operation is going to be performed. The mod on the NTL libraries has to be defined prior these methods are used.
         * @param a value to be multiplied.
         * @param b value to be multiplied.
         * @return c c=a*b mod m (m is given on the configuration)
         * @exception NA
         * @todo
         <ul>
         <li>Build Exceptions for non mod data
         </ul>
         */
        static long multiplyMod(long a, long b);
        

        /**
         * @brief Builds An specification on generic multiplier of polynomials
         * @details calculations are done using NTL libraries.
         * @note all values are also in mod the operation is going to be performed. The mod on the NTL libraries has to be defined prior these methods are used.
         * @param x_v value base of the polynomial.
         * @param x_j size of the plynomial.
         * @return x_p lagrangian polynomial
         * @exception NA
         * @todo
         <ul>
         <li>Build Exceptions for non mod data
         </ul>
         */
        static ZZ_pX multiplyLagrangePolynomials(vec_ZZ_p x_v, ZZ_p x_j);
        
        /**
         * @brief Override Method - Builds An specification on generic multiplier of polynomials. Method for adjacent points only.
         * @details calculations are done using NTL libraries.
         * @note all values are also in mod the operation is going to be performed. The mod on the NTL libraries has to be defined prior these methods are used.
         * @param x_v value base of the polynomial.
         * @param x_j size of the plynomial.
         * @return x_p polynomials
         * @exception NA
         * @todo
         <ul>
         <li>Build Exceptions for non mod data
         </ul>
         */
        static ZZ_pX multiplyLagrangePolynomials(int x_s, int x_k,int x_j);
        
        static int pow2roundup (int value);
        

    };
    
}
#endif
