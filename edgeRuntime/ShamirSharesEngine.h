//
//  ShamirSharesEngine.h
//  edgeRuntime
//
//  Created by Abdelrahaman Aly on 20/11/13.
//  Copyright (c) 2013 Abdelrahaman Aly. All rights reserved.
//

#ifndef edgeRuntime_ShamirSharesEngine_h
#define edgeRuntime_ShamirSharesEngine_h
//Library Headers
#include <NTL/vec_ZZ_p.h>

//Custom Headers
#include "StandardPlayer.h"
#include "StandardShare.h"
#include "SharesManager.h"
#include "ShamirGenerator.h"
#include "SharesListener.h"
#include "List.h"

using namespace NTL;

namespace SmcEngines 
{
    /**
     * @class ShamirSharesEngine
     * @ingroup SmcEngines
     * @brief
     * This class is the central piece of the Framework. Is in charge of Runtime Functionality i.e. (Shamir Sharing Behavior) and the collection of operations that can be performed using the produced shares
     * @details The class uses the parametrization of the application trhough the Utilities::Constants class. It can have several different configuration processes, and is unique for Shamir Sharing. All Shamir Sharing functionality has to be placed.
     *   The Engine act like a Runtime and is the enviroment used to execute operations. To facilitate develpment and agilize the performance, the operations have been also placed with the Engine. A further  step will separate the runtime (the hole operations that do not depend on the type of the share) from the engine. And the runtime should have  a configuration step where the engine is introduced.
     *   The engine functionality uses the framework communicational capabilites to execute Shamir functionallity and the ShareGenerators::ShamirGenerator to obtain shares.
     *   It hsould be a single runtime per application. This forces the process to be sequential, which is also intentional to try to accelerate performance. 
     *   The size of the field is defined by p. For some comparison methods p >> l+k +log(n)
     * @todo
     <ul>
        <li> Separate the Engine From the Runtime. i.e. The Shamir Sharing Functionality from the Shares Functionality
        <li> Use for this methods the interface and make them generic
     <\ul>
     * @author Abdelrahaman Aly
     * @date  2013/11/20
     * @version 0.0.1.15
     */
    class ShamirSharesEngine
    {
    private:
        static double relativeTime;
        Managers::SharesManager * shareManager_; //Single instance of facade functionallity
        ShareGenerators::ShamirGenerator * generator_; //Single instance of shares generator
        Players::StandardPlayer * player_; //player who executes the engine
        bool signed_;//Determines comparisons and reconstruct methods should take into account the sign of an operation
        Utils::List<Players::StandardPlayer>  * players_; //Set of players to whom global communications are sent
        static Listeners::SharesListener * listener_; //Single instance of the listener that belongs to engine
        long p_; //Size of the field of the engine
        int nBits_; //Bitsize of the field (Bitsize of p)
        int l_; //Bitsize of usable numbers of the field l<=nBits
        int k_;  //Security parameter for comparison methods
        vec_ZZ_p alphas_;  //Alpha coefficients for lagrangian interpolation for \cite LT13
        vec_ZZ_p * lAlphas_; //Alpha coefficients for lagrangian interpolations
        bool mode_big_p_;
        ZZ_p big_p_;
        
        //Method Implementation of ShamirSharesEngine
        
        //native operations
        
        /**
         * @brief 
         * Secure Basic Addition process.
         * @details 
         * Method calls the Util implementation of Field Addition on instantiated field.
         * @param a number to be added
         * @param b number to be added
         * @return c = a+b
         * @exception NA
         * @todo
         <ul>
         <li>Build Exceptions for field management.
         </ul>
         */
        long addSecure(long a, long  b);
        
        /**
         * @brief
         * Secure Basic Multiplication process.
         * @details
         * Method calls the Util implementation of Field Multiplication on instantiated field.
         * @param a number to be added
         * @param b number to be added
         * @return c = a*b
         * @exception NA
         * @todo
         <ul>
         <li>Build Exceptions for field management.
         </ul>
         */
        long multiplySecure(long a, long  b);
        
        /**
         * @brief
         * Decoupled Secure Basic Addition process in case any especial procedure has to be taken into account while adding scalars to shared values.
         * @details
         * Method calls the Util implementation of Field Addition on instantiated field.
         * @param a number to be added.
         * @param shareValue shared value to be added
         * @return c = a+shareValue
         * @exception NA
         * @todo
         <ul>
         <li> Implement method
         <li>Build Exceptions for field management.
         </ul>
         */
        long addScalarSecure(long  a, long  shareValue);
        
        
        /**
         * @brief
         * Decoupled Secure Basic Multiplication process in case any especial procedure has to be taken into account while adding scalars to shared values.
         * @details
         * Method calls the Util implementation of Field Multiplication on instantiated field.
         * @param a number to be added.
         * @param shareValue shared value to be added
         * @return c = a*shareValue
         * @exception NA
         * @todo
         <ul>
         <li>Build Exceptions for field management.
         </ul>
         */
        long substractSecure(long  a, long  b);


        //bitwise operations
        
        /**
         * @brief
         * Carry Mechanism explained by \cite BLW08, for next bit.
         * @details
         * Uses xor operations to determine next bit is going to need a carry, can be used by any addition method that uses carry bits.
         * @param a one shared bit that is going to be added.
         * @param b one shared bit that is going to be added.
         * @param c one shared bit that is being carry from previous operation.
         * @return carry = (a*b) xor (b,c) xor (a*c)
         * @exception NA
         * @todo
         <ul>
         <li> Simplify the expression being operated to b*(a xor c) xor (a*c)
         <li>Build Exceptions for field management.
         </ul>
         */
        Shares::StandardShare * carrySharemindSecure(Shares::StandardShare * a, Shares::StandardShare * b,Shares::StandardShare * c);
        
        /**
         * @brief
         * Performs addition as explained by \cite DKNT06, for two Util::List of bits using a carry list as well and then return a Util::List of the result.
         * @details
         * Uses carrySharemind to implement secure addition and maintains that LSB is on the leftmost position for all operations and returned data
         * @param a one shared bit-list that is going to be added.
         * @param b one shared bit-list that is going to be added.
         * @param c one shared bit-list that is being carry from previous operation.
         * @return sum =a+b
         * @exception NA
         * @todo
         <ul>
         <li>Build Exceptions for field management.
         </ul>
         */
        Utils::List<Shares::StandardShare> * bitwiseAdditionSecure(Utils::List<Shares::StandardShare> * a,Utils::List<Shares::StandardShare> *b,Utils::List<Shares::StandardShare> * c);
        
        /**
         * @brief
         * Transforms with succesive powers a bit shared form of a number into a decimal.
         * @details
         * Uses no communicational rounds to transform a bit represented number into its decimal equivalent. Maintains that LSB is on the leftmost position for all operations and returned data.
         * @param bits is the list of bits to be transformed.
         * @param size number of bits to be used for the transformation.
         * @return decimal shared  equivalent to bits.
         * @exception NA
         * @todo
         <ul>
         <li>Build Exceptions for field management.
         </ul>
         * @bug
         <ul>
         <li> For l bigger than 16 (32 forward) the previous implementation - with an int pow- cannot be sustain. The bug was detected the 9th of May of 2014 at the Copenhaguen airport. the bug was detected because the random number that were using this function where showing this wrong result
         </ul>
         */
        Shares::StandardShare * btod(Utils::List<Shares::StandardShare> * bin, int size);
        
        /**
         * @brief
         * Detects the position of the most significative bit and returns a vector full of 0 and a 1 on the position where the most significative bit is located. This method is an implementation of the algorithm explained by \cite DKNT06.
         * @details
         * Explores the bit Utils::List from the MSB to the LSB. Puts 0 until 1 is found then, introduces a 1, and complete the rest with 0.
         * @return list e.g. [0,0,0,0,1,1,1,1]=[0,0,0,0,1,0,0,0]
         * @exception NA
         * @todo
         <ul>
         <li>Introduce a size parameter.
         <li>Build Exceptions for field management.
         <li>decouple basic functionality -the building of the result vector
         </ul>
         */
        Utils::List<Shares::StandardShare> *  obtainMostSignificativeBit(Utils::List<Shares::StandardShare> * c);
        
        /**
         * @brief
         * Calculates securely the complement 2 of a number by adding  one to it and then decomosing it into its bit shares.
         * @details
         * Re-use existing complement 2 methods. It works on non-secure data.
         * @param value coeficient from which the component 2 is going to be calculated.
         * @param l number of bits to which the numbers are going to be calculated.
         * @return list e.g. 5 =101
         * @exception NA
         * @todo
         <ul>
         <li>Use randomization to easily calculate the bit decomposition as in \cite Toft13.
         <li>Build Exceptions for field management.
         </ul>
         */
        Utils::List<Shares::StandardShare> *  obtainComplementTwoInShares(long value, int l);
        
        //comparison operations
        
        /**
         * @brief
         * This method follows Toft's implementation on \cite Toft13. It strictly follows and uses the methods the original paper suggests, including all the \cite DKNT06 reference on Toft's paper for inverse random numbers and random generation numbers.
         * @details
         * k is a statistical parameter to five reliability to the method. In this case it replaces modular arithmetic. as for this methods is necessary that 2^l+k+ln(n)<<M then the only assuranceis the size of the k.
         * @param a value to be compared with
         * @param b value to be compared against
         * @param ap id player of share a.
         * @param bp id player of share b.
         * @param l number of bits to which the numbers are going to be calculated.
         * @return share a>b e.g. 2>3=0
         * @exception NA
         * @todo
         <ul>
         <li>Use randomization to easily calculate the bit decomposition as in \cite Toft13.
         <li>Build Exceptions for field management.
         </ul>
         */
        Shares::StandardShare * greaterThanlBitsShares(Shares::StandardShare * a, Shares::StandardShare * b,int ap, int bp, int l);
        
        //transmission related methods
        
        /**
         * @brief
         * Interpolate shares and reveals its value.
         * @details
         * uses lagrangian interpolation to reveal the secret value encoded in the list of shares.
         * @param shares shamir shares to be interpolated
         * @return value interpolated value
         * @exception NA
         * @todo
         <ul>
         <li>The method is set to work only with all players, define the method in such a way it allows custom sets of players.
         <li>Build Exceptions for field management.
         </ul>
         */
        long interpolateValues(Utils::List<Shares::StandardShare> * shares);
        
    public:
        int totalCom=0;
        bool is_gib_p();
        //Constructors
        
        /**
         * @brief
         * Custom Constructor. Receives estate variables and executes configuration process.
         * @details
         * Instantiate the alpha vectors for lagrangian regression, set estate variables from the application parametrization and initialize the field on which the engine is going to work, are the taks executed by this constructor. It also initialize the arrays
         * @param player player who executes the application.
         * @param players players involved in the communication.
         * @exception NA
         * @todo
         <ul>
         <li>Use the generic initialization process for the buffers instead of doing it manually.
         <li>Build Exceptions for field management.
         </ul>
         */
        ShamirSharesEngine(Players::StandardPlayer * player, Utils::List<Players::StandardPlayer>  * players, bool signed_);
        Players::StandardPlayer * getPlayer();
        
        //Random number generation methods
        
        /**
         * @brief
         * Generates random number with a naive approach.
         * @details
         * Asks every player to contribute with a secret shared random number and then adds them. Uses the size of the field as threshold and the addition operations are also performed with field additions.
         * @note The threshold is an upper bound on the locally calculated random number. Not on the multiparty calculated random number.
         * @return random secret shared number.
         * @exception NA
         * @todo
         <ul>
         <li>Build Exceptions for field management.
         </ul>
         */
        Shares::StandardShare * generateShareRandomNumber();
        
        /**
         * @brief
         * Generates random number with a naive approach using custom thereshold.
         * @details
         * Asks every player to contribute with a secret shared random number and then adds them. Uses a parametrized threshold and the addition operations are also performed with field additions.
         * @param thereshold custom upper bound on the size of the random number generated.
         * @return random secret shared number.
         * @exception NA
         * @todo
         <ul>
         <li>Build Exceptions for field management.
         </ul>
         */
        Shares::StandardShare * generateShareRandomNumber(long threshold);
        
        /**
         * @brief
         * Uses \cite DKNT06 to generate a shared random bit.
         * @details
         * A random shared bit is generated using 2 communicational rounds and ifeld properties as explained by \cite DKNT06.
         * @return random secret shared bit.
         * @exception NA
         * @todo
         <ul>
         <li>Build Exceptions for field management.
         </ul>
         */
        Shares::StandardShare * generateDagmardRandomBit();
        
        
        /**
         * @brief
         * Uses \cite DKNT06 to generate a shared random number.
         * @details
         * A random shared number is generated using the bit generation technique rounds and ifeld properties as explained by \cite DKNT06. The parameters are instantiated with pointers to the corresponding data, and the status is returned.
         * @param number resulting decimal shared form of random number generated.
         * @param bitwiseNumber bit decomposition of the resulting random number.
         * @param l quantity of random bits generated.
         * @return status indicates success or faliure.
         * @exception NA
         * @todo
         <ul>
         <li> Obtain bits should be a function parameter setted up during the configuration process. looses time in the function.
         <li>Build Exceptions for field management.
         </ul>
         */
        int generateDagmardBitwiseRandomNumber(Shares::StandardShare ** number, Utils::List<Shares::StandardShare> ** bitwiseNumber, int l);
        
        /**
         * @brief
         * Uses a naive approach to generate a shared random number.
         * @details
         * Performs a bit addition of the locally generated random shared numbers to obtain its bit decomposition
         * @param number resulting decimal shared form of random number generated.
         * @param bitwiseNumber bit decomposition of the resulting random number.
         * @param l quantity of random bits generated.
         * @return status indicates success or faliure.
         * @exception NA
         * @todo
         <ul>
         <li> Obtain bits should be a function parameter setted up during the configuration process. looses time in the function.
         <li>Build Exceptions for field management.
         </ul>
         */
        int generateBitwiseRandomNumber(Shares::StandardShare ** number, Utils::List<Shares::StandardShare> ** bitwiseNumber, int l);

        
        //Binary Operation Methods
        
        /**
         * @brief
         * Transforms with succesive powers a bit shared form of a number into a decimal.
         * @details
         * Uses no communicational rounds to transform a bit represented number into its decimal equivalent. Maintains that LSB is on the leftmost position for all operations and returned data.
         * @param a list of shared bits to be transformed
         * @return decimal shared  equivalent to bits.
         * @exception NA
         * @todo
         <ul>
         <li>Build Exceptions for field management.
         </ul>
         * @bug
         <ul>
         <li> For l bigger than 16 (32 forward) the previous implementation - with an int pow- cannot be sustain. The bug was detected the 9th of May of 2014 at the Copenhaguen airport. the bug was detected because the random number that were using this function where showing this wrong result
         </ul>
         */
        Shares::StandardShare * obtainDecimalShareFromBits(Utils::List<Shares::StandardShare> * a);
        
        /**
         * @brief
         * Carry Mechanism explained by \cite BLW08, to calculate the carry vector.
         * @details
         * Uses xor operations to determine next bit is going to need a carry, can be used by any addition method that uses carry bits.
         * @param a list of shared bits that is going to be added.
         * @param b list of shared bits that is going to be added.
          * @return carryVector  carry vector corresponding to a+b
         * @exception NA
         * @todo
         <ul>
         <li>Build Exceptions for field management.
         </ul>
         */
        Utils::List<Shares::StandardShare> * carrySharemindShares(Utils::List<Shares::StandardShare> * a, Utils::List<Shares::StandardShare> * b);
        
        /**
         * @brief
         * Performs addition as explained by \cite DKNT06, for two Util::List of bits using a carry list as well and then return a Util::List of the result. using the implemented addition methodologies
         * @details
         * Currently it can only use \cite BLW08 carry Sharemind method to implement secure addition and maintains that LSB is on the leftmost position for all operations and returned data
         * @param a one shared bit-list that is going to be added.
         * @param b one shared bit-list that is going to be added.
         * @param type signals which addition method is going to be used.
         * @return sum =a+b
         * @exception NA
         * @todo
         <ul>
         <li>Build Exceptions for field management.
         </ul>
         */
        Utils::List<Shares::StandardShare> * bitwiseAdditionShares(Utils::List<Shares::StandardShare> * a,Utils::List<Shares::StandardShare> * b,int type);
        
        //this method uses double pointers given the way c++ returns values it was the naive approach to use them
        /**
         * @brief
         * Decompose a secet share into secret bits as indicated by \cite DKNT06 
         * @details
         * Calculates a random number using provided methods and uses it to decompose the number into bits. The LSB is on the first position from the left.
         * @param share value in shared form to be decomposed.
         * @return bits  [5]=[101]
         * @exception NA
         * @todo
         <ul>
         <li>Build Exceptions for field management.
         </ul>
         */
        Utils::List<Shares::StandardShare> *  obtainBitsFromShare(Shares::StandardShare * share);
        
        //returns a>b in shareForm
        /**
         * @brief
         * Compares the bit forms of to shares and determines a>b.
         * @details
         * This method follows the implementation on \cite DKNT06. It strictly follows and uses the methods the original paper suggests. No bitsize is needed given that the size of a is used for that matter.
         * @param a shared value to be compared with
         * @param b shared value to be compared against
         * @return share a>b e.g. 2>3=0
         * @exception NA
         * @todo
         <ul>
         <li>Use randomization to easily calculate the bit decomposition as in \cite Toft13.
         <li>Build Exceptions for field management.
         </ul>
         */
        Shares::StandardShare * bitwiseGreaterThanSecure(Utils::List<Shares::StandardShare> * a,Utils::List<Shares::StandardShare> * b);

        /**
         * @brief
         * Compares the bit forms of a share versus a scalar and determines a<=scalar.
         * @details
         * This method is an adaptation of \cite DKNT06 modified by Aly et al. It follows and uses the methods the original paper suggests. No bitsize is needed given that the size of a is used for that matter.
         * @param a shared value to be compared with
         * @param b scalar to be compared against
         * @return share a<=b e.g. 2<3=1
         * @exception NA
         * @todo
         <ul>
         <li>Use randomization to easily calculate the bit decomposition as in \cite Toft13.
         <li>Build Exceptions for field management.
         </ul>
         */
        Shares::StandardShare * bitwiseLessThanEqualSecureScalar(Utils::List<Shares::StandardShare> * a,long b);
        
        /**
         * @brief
         * Compares the bit forms of a share versus a scalar and determines a<scalar.
         * @details
         * This method is an adaptation of \cite DKNT06. It follows and uses the methods the original paper suggests. No bitsize is needed given that the size of a is used for that matter.
         * @param a shared value to be compared with
         * @param b scalar to be compared against
         * @return share a<b e.g. 2<3=1
         * @exception NA
         * @todo
         <ul>
         <li>Use randomization to easily calculate the bit decomposition as in \cite Toft13.
         <li>Build Exceptions for field management.
         </ul>
         */
        Shares::StandardShare * bitwiseLessThanSecureScalar(Utils::List<Shares::StandardShare> * a,long b);

        /**
         * @brief
         * Compares the bit forms of a share versus a scalar and determines scalar<a minimizing complexity to leves of a<scalar. Modification of Aly et al.
         * @details
         * This method is an adaptation of \cite DKNT06. It follows and uses the methods the original paper suggests. No bitsize is needed given that the size of a is used for that matter.
         * @param a shared value to be compared with
         * @param b scalar to be compared against
         * @return share a<b e.g. 2<3=1
         * @exception NA
         * @todo
         <ul>
         <li>Use randomization to easily calculate the bit decomposition as in \cite Toft13.
         <li>Build Exceptions for field management.
         </ul>
         */
        Shares::StandardShare * bitwiseLessThanSecureScalar(long b, Utils::List<Shares::StandardShare> * a);
        
        //Basic Operation Methods
        
        /**
         * @brief
         * Secure Basic Addition process between share and scalar.
         * @details
         * Method calls the Util implementation of Field Addition on instantiated field.
         * @param a share to be added
         * @param b number to be added
         * @return c = a+b
         * @exception NA
         * @todo
         <ul>
         <li>Build Exceptions for field management.
         </ul>
         */
        Shares::StandardShare * addScalar(Shares::StandardShare * a, long b);
        
        /**
         * @brief
         * Secure Basic Multiplication process between share and scalar.
         * @details
         * Method calls the Util implementation of Field Multiplication on instantiated field.
         * @param a share to be multiplied
         * @param b number to be multiplied
         * @return c = a*b
         * @exception NA
         * @todo
         <ul>
         <li>Build Exceptions for field management.
         </ul>
         */
        Shares::StandardShare * multiplyScalar(Shares::StandardShare * a, long b);
        
        /**
         * @brief
         * Secure Basic Xor process between share and scalar.
         * @details
         * Method calls the Util implementation of Field Xor on instantiated field.
         * @note this operation is costless -no communicational rounds needed
         * @param a share to be xor
         * @param b number to be xor
         * @return c = a xor b
         * @exception NA
         * @todo
         <ul>
         <li>Build Exceptions for field management.
         </ul>
         */
        Shares::StandardShare * xorScalar(Shares::StandardShare * a, long b);
        
        /**
         * @brief
         * Secure Basic Substraction process between share and scalar.
         * @details
         * Method calls the Util implementation of Field Substraction on instantiated field.
         * @param a share to be substracted from
         * @param b number to be substracted to
         * @return c = a-b
         * @exception NA
         * @todo
         <ul>
         <li>Build Exceptions for field management.
         </ul>
         */
        Shares::StandardShare * substractScalar(Shares::StandardShare * a, long b);
        
        /**
         * @brief
         * Secure Basic Substraction process between scalar and a share.
         * @details
         * Method calls the Util implementation of Field Substraction on instantiated field.
         * @param a number to be substracted from
         * @param b share to be substracted to
         * @return c = a-b
         * @exception NA
         * @todo
         <ul>
         <li>Build Exceptions for field management.
         </ul>
         */
        Shares::StandardShare * substractScalar(long b, Shares::StandardShare * a);
        
        //Basic Shares operation Methods
        
        /**
         * @brief
         * Secure Basic Addition process between  2 shares.
         * @details
         * Method calls the Util implementation of Field Addition on instantiated field.
         * @param a share to be added.
         * @param b share to be added.
         * @return c = a+b
         * @exception NA
         * @todo
         <ul>
         <li>Build Exceptions for field management.
         </ul>
         */
        Shares::StandardShare * addShares(Shares::StandardShare *a,Shares::StandardShare *b);
        
        /**
         * @brief
         * Secure Basic Substraction process between  2 shares.
         * @details
         * Method calls the Util implementation of Field Substraction on instantiated field.
         * @param a share to be substracted from.
         * @param b share to be substracted to.
         * @return c = a-b
         * @exception NA
         * @todo
         <ul>
         <li>Build Exceptions for field management.
         </ul>
         */
        Shares::StandardShare * substractShares(Shares::StandardShare *a,Shares::StandardShare *b);
        
        /**
         * @brief
         * Secure Basic Multiplication process between  2 shares.
         * @details
         * Method calls the Util implementation of Field Multiplication on instantiated field.
         * @param a share to be multiplied.
         * @param b share to be multiplied.
         * @return c = a*b
         * @exception NA
         * @todo
         <ul>
         <li>Build Exceptions for field management.
         </ul>
         */
        Shares::StandardShare * multiplyShares(Shares::StandardShare *a,Shares::StandardShare *b);
        
        /**
         * @brief
         * Secure Basic Inversion of a share described by \cite Toft13.
         * @details
         * Uses randomization to securely obtain the multiplicative inverse of a secretly shared number  in a finite field.
         * @note the multiplicative inverse is the number for which the share once multiplied is 1 in the finite field.
         * @param a share to be inverted.
         * @return i = a^(-1)
         * @exception NA
         * @todo
         <ul>
         <li>Build Exceptions for field management.
         </ul>
         */
        Shares::StandardShare * invertShare(Shares::StandardShare *a);
        
        /**
         * @brief
         * Secure mode 2^m of a share described by \cite CH10.
         * @details
         * Uses randomization to securely obtain the mod of a secretly shared number.
         * @note the value is calculated
         * @param a share to be inverted.
         * @return a =a mod 2^m
         * @exception NA
         * @todo
         <ul>
         <li>Build Exceptions for field management.
         </ul>
         */
        Shares::StandardShare * mod2m(Shares::StandardShare * a, int k, int m);
        
        /**
         * @brief
         * Secure Basic Exponentiation process of a share to and integer power.
         * @details
         * Method calls the Util implementation of Field Multiplication on instantiated field.
         * Power of 0 that is equal  to 1 is excluded from the list.
         * @param a share to be used as base.
         * @param e number to be used as exponent.
         * @return c = a^e on index (e-1).
         * @exception NA
         * @todo
         <ul>
         <li>Build Exceptions for field management.
         </ul>
         */
        Utils::List<Shares::StandardShare> * powerShare(Shares::StandardShare * share, int e);
        
        /**
         * @brief
         * Secure Basic fan in  process for a collection of shares.
         * @details
         * generates a collection full of the fan in multiplication results of the submitted shares.
         * @param shares collection of shares to be fan in.
         * @return list [[a],[b],[c]] = [[a], [a*b], [a*b*c]]
         * @exception NA
         * @todo
         <ul>
         <li>Build Exceptions for field management.
         </ul>
         */
        Utils::List<Shares::StandardShare> * fanInMultiplicationShares(Utils::List<Shares::StandardShare> * shares);
        
        //secure methods: polymorphism
        
        /**
         * @brief
         * Polymorphic form of addition. Obtains a secure addition process between 2 shares.
         * @details
         * Override
         * Method overrides the data type of the parameters. i.e. Same name method for operations with shares and scalars.
         * All operations are executed on a finite field and rehuse addition methods.
         * Its creation was motivated by rehusability.
         * @param a share to be added.
         * @param b share to be added.
         * @return c = a+b
         * @exception NA
         * @todo
         <ul>
         <li>Build Exceptions for field management.
         </ul>
         */
        Shares::StandardShare * add(Shares::StandardShare *a,Shares::StandardShare *b);
        
        /**
         * @brief
         * Polymorphic form of addition. Obtains a secure addition process between a share and a number.
         * @details
         * Override
         * Method overrides the data type of the parameters. i.e. Same name method for operations with shares and scalars.
         * All operations are executed on a finite field and rehuse addition methods.
         * Its creation was motivated by rehusability.
         * @param a share to be added.
         * @param b number to be added.
         * @return c = a+b
         * @exception NA
         * @todo
         <ul>
         <li>Build Exceptions for field management.
         </ul>
         */
        Shares::StandardShare * add(Shares::StandardShare * a, long b);
        
        /**
         * @brief
         * Polymorphic form of addition. Obtains a secure addition process between a share and itself.
         * @details
         * Override
         * Method overrides the data type of the parameters. i.e. Same name method for operations with shares and scalars.
         * All operations are executed on a finite field and rehuse addition methods.
         * Its creation was motivated by rehusability.
         * @param a share to be added.
         * @param b number to be added.
         * @return a += b
         * @exception NA
         * @todo
         <ul>
         <li>Build Exceptions for field management.
         </ul>
         */
        Shares::StandardShare * addTo(Shares::StandardShare * a, Shares::StandardShare * b);
        
        /**
         * @brief
         * Polymorphic form of addition. Obtains a secure addition process between a number and itself.
         * @details
         * Override
         * Method overrides the data type of the parameters. i.e. Same name method for operations with shares and scalars.
         * All operations are executed on a finite field and rehuse addition methods.
         * Its creation was motivated by rehusability.
         * @param a share to be added.
         * @param b number to be added.
         * @return a += b
         * @exception NA
         * @todo
         <ul>
         <li>Build Exceptions for field management.
         </ul>
         */
        Shares::StandardShare * addTo(Shares::StandardShare * a, long b);

        
        /**
         * @brief
         * Polymorphic form of substraction. Obtains a secure substraction process between 2 shares.
         * @details
         * Override
         * Method overrides the data type of the parameters. i.e. Same name method for operations with shares and scalars.
         * All operations are executed on a finite field and rehuse substraction methods.
         * Its creation was motivated by rehusability.
         * @param a share to be substracted from.
         * @param b share to be substracted to.
         * @return c = a-b
         * @exception NA
         * @todo
         <ul>
         <li>Build Exceptions for field management.
         </ul>
         */
        Shares::StandardShare * substract(Shares::StandardShare *a,Shares::StandardShare *b);
        
        /**
         * @brief
         * Polymorphic form of substraction. Obtains a secure substraction process between a share and a scalar.
         * @details
         * Override
         * Method overrides the data type of the parameters. i.e. Same name method for operations with shares and scalars.
         * All operations are executed on a finite field and rehuse substraction methods.
         * Its creation was motivated by rehusability.
         * @param a share to be substracted from.
         * @param b number to be substracted to.
         * @return c = a-b
         * @exception NA
         * @todo
         <ul>
         <li>Build Exceptions for field management.
         </ul>
         */
        Shares::StandardShare * substract(Shares::StandardShare * a, long b);
        
        /**
         * @brief
         * Polymorphic form of substraction. Obtains a secure substraction process between a scalar and a share.
         * @details
         * Override
         * Method overrides the data type of the parameters. i.e. Same name method for operations with shares and scalars.
         * All operations are executed on a finite field and rehuse substraction methods.
         * Its creation was motivated by rehusability.
         * @param a number to be substracted from.
         * @param b share to be substracted to.
         * @return c = a-b
         * @exception NA
         * @todo
         <ul>
         <li>Build Exceptions for field management.
         </ul>
         */
        Shares::StandardShare * substract( long b, Shares::StandardShare * a);
        
        
        /**
         * @brief
         * Polymorphic form of substraction. Obtains a secure substraction process between a share and itself.
         * @details
         * Override
         * Method overrides the data type of the parameters. i.e. Same name method for operations with shares and scalars.
         * All operations are executed on a finite field and rehuse substraction methods.
         * Its creation was motivated by rehusability.
         * @param a number to be substracted from.
         * @param b share to be substracted to.
         * @return a -=b
         * @exception NA
         * @todo
         <ul>
         <li>Build Exceptions for field management.
         </ul>
         */
        Shares::StandardShare * substractTo( Shares::StandardShare * a, Shares::StandardShare * b);
        
        /**
         * @brief
         * Polymorphic form of substraction. Obtains a secure substraction process between a scalar and a share with the result on itself.
         * @details
         * Override
         * Method overrides the data type of the parameters. i.e. Same name method for operations with shares and scalars.
         * All operations are executed on a finite field and rehuse substraction methods.
         * Its creation was motivated by rehusability.
         * @param a number to be substracted from.
         * @param b share to be substracted to.
         * @return a-= b
         * @exception NA
         * @todo
         <ul>
         <li>Build Exceptions for field management.
         </ul>
         */
        Shares::StandardShare * substractTo( Shares::StandardShare * a, long b);
        
        /**
         * @brief
         * Polymorphic form of substraction. Obtains a secure substraction process between a scalar and a share returning the result on itself.
         * @details
         * Override
         * Method overrides the data type of the parameters. i.e. Same name method for operations with shares and scalars.
         * All operations are executed on a finite field and rehuse substraction methods.
         * Its creation was motivated by rehusability.
         * @param a number to be substracted from.
         * @param b share to be substracted to.
         * @return a-= b
         * @exception NA
         * @todo
         <ul>
         <li>Build Exceptions for field management.
         </ul>
         */
        Shares::StandardShare * substractTo( long b, Shares::StandardShare * a);
        
        
        /**
         * @brief
         * Polymorphic form of multiplications. Obtains a secure multiplication process between  2 shares.
         * @details
         * Override
         * Method overrides the data type of the parameters. i.e. Same name method for operations with shares and scalars.
         * All operations are executed on a finite field and rehuse multiplications methods.
         * Its creation was motivated by rehusability.
         * @param a share to be multiplied.
         * @param b share to be multiplied.
         * @return c = a*b
         * @exception NA
         * @todo
         <ul>
         <li>Build Exceptions for field management.
         </ul>
         */
        Shares::StandardShare * multiply(Shares::StandardShare *a,Shares::StandardShare *b);
        
        /**
         * @brief
         * Polymorphic form of multiplications. Obtains a secure multiplication process between  a share and a long number.
         * @details
         * Override
         * Method overrides the data type of the parameters. i.e. Same name method for operations with shares and scalars.
         * All operations are executed on a finite field and rehuse multiplications methods. 
         * Its creation was motivated by rehusability.
         * @param a share to be multiplied.
         * @param b number to be multiplied.
         * @return c = a*b
         * @exception NA
         * @todo
         <ul>
         <li>Build Exceptions for field management.
         </ul>
         */
        Shares::StandardShare * multiply(Shares::StandardShare * a, long b);

        /**
         * @brief
         * Polymorphic form of multiplications. Obtains a secure multiplication process between  shares returing the result on itself.
         * @details
         * Override
         * Method overrides the data type of the parameters. i.e. Same name method for operations with shares and scalars.
         * All operations are executed on a finite field and rehuse multiplications methods.
         * Its creation was motivated by rehusability.
         * @param a share to be multiplied.
         * @param b number to be multiplied.
         * @return a*=b
         * @exception NA
         * @todo
         <ul>
         <li>Build Exceptions for field management.
         </ul>
         */
        Shares::StandardShare * multiplyTo(Shares::StandardShare * a, Shares::StandardShare * b);

        /**
         * @brief
         * Polymorphic form of multiplications. Obtains a secure multiplication process between  a share and a long number returning the value on itself.
         * @details
         * Override
         * Method overrides the data type of the parameters. i.e. Same name method for operations with shares and scalars.
         * All operations are executed on a finite field and rehuse multiplications methods.
         * Its creation was motivated by rehusability.
         * @param a share to be multiplied.
         * @param b number to be multiplied.
         * @return c = a*b
         * @exception NA
         * @todo
         <ul>
         <li>Build Exceptions for field management.
         </ul>
         */
        Shares::StandardShare * multiplyTo(Shares::StandardShare * a, long b);
        
        /**
         * @brief
         * Secure Basic Xor process between 2 shares.
         * @details
         * Method calls the Util implementation of Field Xor on instantiated field between shares.
         * @note this operation is not costless -a communicational round needed
         * @param a share to be xor
         * @param b share to be xor
         * @return c = a xor b
         * @exception NA
         * @todo
         <ul>
         <li>Build Exceptions for field management.
         </ul>
         */
        Shares::StandardShare * sxor(Shares::StandardShare * a, Shares::StandardShare * b);
        
        /**
         * @brief
         * Secure Basic Xor process between 2 shares.
         * @details
         * Method calls the Util implementation of Field Xor on instantiated field between shares.
         * @note this operation is not costless -a communicational round needed
         * @param a share to be xor
         * @param b share to be xor
         * @return c = a xor b
         * @exception NA
         * @todo
         <ul>
         <li>Build Exceptions for field management.
         </ul>
         */
        Shares::StandardShare * sxor(Shares::StandardShare * a, long b);
        
        /**
         * @brief
         * Secure Basic Xor process between 2 shares and return the result on itself.
         * @details
         * Method calls the Util implementation of Field Xor on instantiated field.
         * @note this operation is not costless -a communicational round needed
         * @param a share to be xor
         * @param b share to be xor
         * @return a xor = b
         * @exception NA
         * @todo
         <ul>
         <li>Build Exceptions for field management.
         </ul>
         */
        Shares::StandardShare * xorTo(Shares::StandardShare * a, Shares::StandardShare * b);
        
        /**
         * @brief
         * Secure Basic Xor process between 2 shares and return it on itself.
         * @details
         * Method calls the Util implementation of Field Xor on instantiated field.
         * @note this operation is not costless -a communicational round needed
         * @param a share to be xor
         * @param b share to be xor
         * @return a xor= b
         * @exception NA
         * @todo
         <ul>
         <li>Build Exceptions for field management.
         </ul>
         */
        Shares::StandardShare * xorTo(Shares::StandardShare * a,  bool b);
        
        
        
        
        //public applications
        /**
         * @brief
         * Secure Basic Xor process between 2 shares.
         * @details
         * Method calls the Util implementation of Field Xor on instantiated field.
         * @note this operation is not costless -a communicational round needed
         * @param a share to be xor
         * @param b share to be xor
         * @return c = a xor b
         * @exception NA
         * @todo
         <ul>
         <li>Build Exceptions for field management.
         </ul>
         */
        Shares::StandardShare * xorShares(Shares::StandardShare * a, Shares::StandardShare * b);
        /**
         * @brief
         * This method follows Damgard et al. implementation on \cite DKNT06. It strictly follows and uses the methods the original paper suggests, including all the \cite DKNT06 references.
         * @details
         * uses bit decomposition of the shares before calculation, in case the bit decomposition has been previously performed it reuses the result.
         * @param a value to be compared with
         * @param b value to be compared against
         * @return share a>b e.g. 2>3=0
         * @exception NA
         * @todo
         <ul>
         <li>Optimize the method as much as possible.
         <li>Build Exceptions for field management.
         </ul>
         */
        Shares::StandardShare * greaterThanShares(Shares::StandardShare * a, Shares::StandardShare * b);
        
        /**
         * @brief
         * Executes a zero test. This method follows Toft's implementation on \cite Toft13. It strictly follows and uses the methods the original paper suggests, including all the \cite DKNT06 reference on Toft's paper for inverse random numbers and random generation numbers.
         * @details
         * k is a statistical parameter to give reliability to the method. In this case it replaces modular arithmetic. as for this methods is necessary that 2^l+k+ln(n)<<M then the only assuranceis the size of the k.
         * @param a value to be compared with
         * @param l number of bits to which the numbers are going to be calculated.
         * @return share a==0 e.g.1==0=0
         * @exception NA
         * @todo
         <ul>
         <li>Use randomization to easily calculate the bit decomposition as in \cite Toft13.
         <li>Build Exceptions for field management.
         </ul>
         */
        Shares::StandardShare * zeroTestShares(Shares::StandardShare * a, int l);
        
        /**
         * @brief
         * Executes a zero test. This method follows Catrina's implementation on \cite CH10. It strictly follows and uses the methods the original paper suggests, including all the \cite DKNT06 reference on Toft's paper for inverse random numbers and random generation numbers.
         * @details
         * k is a statistical parameter to give reliability to the method. In this case it replaces modular arithmetic. as for this methods is necessary that 2^l+k+ln(n)<<M then the only assuranceis the size of the k.
         * @param a value to be compared with
         * @param l number of bits to which the numbers are going to be calculated.
         * @return share a==0 e.g.1==0=0
         * @exception NA
         * @todo
         <ul>
         <li>Use randomization to easily calculate the bit decomposition as in \cite Toft13.
         <li>Build Exceptions for field management.
         </ul>
         */
        Shares::StandardShare * zeroTestXorShares(Shares::StandardShare * a);
        
        /**
         * @brief
         * Executes an equality test rehusing the zero test method.
         * @details
         * This method follows Toft's implementation on \cite Toft13. It strictly follows and uses the methods the original paper suggests, including all the \cite DKNT06 reference on Toft's paper for inverse random numbers and random generation numbers.
         * k is a statistical parameter to give reliability to the method. In this case it replaces modular arithmetic. as for this methods is necessary that 2^l+k+ln(n)<<M then the only assuranceis the size of the k.
         * @param a value to be compared with
         * @param b value to be compared against
         * @param l number of bits to which the numbers are going to be calculated.
         * @return share a==b (a-b)==0 e.g.1==2=0
         * @exception NA
         * @todo
         <ul>
         <li>Build Exceptions for field management.
         </ul>
         */
        Shares::StandardShare * equalToToftShares(Shares::StandardShare * a, Shares::StandardShare * b,  int l);
        
        /**
         * @brief
         * Executes an equality test rehusing the zero test method using default bitsize.
         * @details
         * default bitsize is defined by the application parametrization, tTheoretically speaking, the bitsize of M.
         * This method follows Toft's implementation on \cite Toft13. It strictly follows and uses the methods the original paper suggests, including all the \cite DKNT06 reference on Toft's paper for inverse random numbers and random generation numbers.
         * k is a statistical parameter to give reliability to the method. In this case it replaces modular arithmetic. as for this methods is necessary that 2^l+k+ln(n)<<M then the only assuranceis the size of the k.
         * @param a value to be compared with
         * @param b value to be compared against
         * @return share a==b (a-b)==0 e.g.1==2=0
         * @exception NA
         * @todo
         <ul>
         <li>Build Exceptions for field management.
         </ul>
         */
        Shares::StandardShare * equalToToftShares(Shares::StandardShare * a, Shares::StandardShare * b);//Toft 2013
        
        /**
         * @brief
         * Executes a greater than comparison. This method follows Toft's implementation on \cite Toft13. It strictly follows and uses the methods the original paper suggests, including all the \cite DKNT06 reference on Toft's paper for inverse random numbers and random generation numbers.
         * @details
         * k is a statistical parameter to give reliability to the method. In this case it replaces modular arithmetic. as for this methods is necessary that 2^l+k+ln(n)<<M then the only assuranceis the size of the k.
         * @param a value to be compared with.
         * @param b value to be compared against.
         * @return share a>b e.g.1>=2=0
         * @exception NA
         * @todo
         <ul>
         <li>Use randomization to easily calculate the bit decomposition as in \cite Toft13.
         <li>Build Exceptions for field management.
         </ul>
         */
        Shares::StandardShare * greaterEqualThanToftShares(Shares::StandardShare * a, Shares::StandardShare * b);//Toft 2013

        /**
         * @brief
         * Executes a less than comparison to 0. This method follows Catrina's implementation  of mod on \cite CH10 and implements the comparitions wihtout extra multiplications. It strictly follows and uses the methods the original paper suggests, including all the \cite DKNT06 reference on Toft's paper for inverse random numbers and random generation numbers.
         * @details
         * k is a statistical parameter to give reliability to the method. In this case it replaces modular arithmetic. as for this methods is necessary that 2^l+k+ln(n)<<M then the only assuranceis the size of the k.
         * @param a value to be compared with.
         * @param b value to be compared against.
         * @return share a<0 e.g.1<0=0
         * @exception NA
         * @todo
         <ul>
         <li>Use randomization to easily calculate the bit decomposition as in \cite Toft13.
         <li>Build Exceptions for field management.
         </ul>
         */
        Shares::StandardShare * lessThanZeroCatrinaModShares(Shares::StandardShare * a);//Catrina Modop. 2014
        
        /**
         * @brief
         * Executes a less than comparison. This method follows Catrina's implementation  of mod on \cite CH10 and implements the comparitions wihtout extra multiplications. It strictly follows and uses the methods the original paper suggests, including all the \cite DKNT06 reference on Toft's paper for inverse random numbers and random generation numbers.
         * @details
         * k is a statistical parameter to give reliability to the method. In this case it replaces modular arithmetic. as for this methods is necessary that 2^l+k+ln(n)<<M then the only assuranceis the size of the k.
         * @param a value to be compared with.
         * @param b value to be compared against.
         * @return share a<b e.g.1<2=1
         * @exception NA
         * @todo
         <ul>
         <li>Use randomization to easily calculate the bit decomposition as in \cite Toft13.
         <li>Build Exceptions for field management.
         </ul>
         */
        Shares::StandardShare * lessThanCatrinaModShares(Shares::StandardShare * a, Shares::StandardShare * b);//Catrina Modop. 2014
        
        
        /**
         * @brief
         * Executes a greater than comparison. This method follows Catrina's implementation  of mod on \cite CH10 and implements the comparitions wihtout extra multiplications. It strictly follows and uses the methods the original paper suggests, including all the \cite DKNT06 reference on Toft's paper for inverse random numbers and random generation numbers.
         * @details
         * k is a statistical parameter to give reliability to the method. In this case it replaces modular arithmetic. as for this methods is necessary that 2^l+k+ln(n)<<M then the only assuranceis the size of the k.
         * @param a value to be compared with.
         * @param b value to be compared against.
         * @return share a>b e.g.1>2=0
         * @exception NA
         * @todo
         <ul>
         <li>Use randomization to easily calculate the bit decomposition as in \cite Toft13.
         <li>Build Exceptions for field management.
         </ul>
         */
        Shares::StandardShare * greaterThanCatrinaModShares(Shares::StandardShare * a, Shares::StandardShare * b);//Catrina Modop. 2014
        
        /**
         * @brief
         * Executes a greater than comparison. This method follows Catrina's implementation  of mod on \cite CH10 and implements the comparitions wihtout extra multiplications. It strictly follows and uses the methods the original paper suggests, including all the \cite DKNT06 reference on Toft's paper for inverse random numbers and random generation numbers.
         * @details
         * k is a statistical parameter to give reliability to the method. In this case it replaces modular arithmetic. as for this methods is necessary that 2^l+k+ln(n)<<M then the only assuranceis the size of the k.
         * @param a value to be compared with.
         * @param b value to be compared against.
         * @return share a<=b e.g.1<=2=0
         * @exception NA
         * @todo
         <ul>
         <li>Use randomization to easily calculate the bit decomposition as in \cite Toft13.
         <li>Build Exceptions for field management.
         </ul>
         */
        Shares::StandardShare * lessEqualThanCatrinaModShares(Shares::StandardShare * a, Shares::StandardShare * b);//Catrina Modop. 2014
        
        /**
         * @brief
         * Executes a greater equal than comparison. This method follows Catrina's implementation  of mod on \cite CH10 and implements the comparitions wihtout extra multiplications. It strictly follows and uses the methods the original paper suggests, including all the \cite DKNT06 reference on Toft's paper for inverse random numbers and random generation numbers.
         * @details
         * k is a statistical parameter to give reliability to the method. In this case it replaces modular arithmetic. as for this methods is necessary that 2^l+k+ln(n)<<M then the only assuranceis the size of the k.
         * @param a value to be compared with.
         * @param b value to be compared against.
         * @return share a>=b e.g.1>=2=0
         * @exception NA
         * @todo
         <ul>
         <li>Use randomization to easily calculate the bit decomposition as in \cite Toft13.
         <li>Build Exceptions for field management.
         </ul>
         */
        Shares::StandardShare * greaterEqualThanCatrinaModShares(Shares::StandardShare * a, Shares::StandardShare * b);//Catrina Modop. 2014
        
        /**
         * @brief
         * Executes a greater than comparison. This method follows Toft's implementation on \cite Toft13. It strictly follows and uses the methods the original paper suggests, including all the \cite DKNT06 reference on Toft's paper for inverse random numbers and random generation numbers.
         * @details
         * k is a statistical parameter to give reliability to the method. In this case it replaces modular arithmetic. as for this methods is necessary that 2^l+k+ln(n)<<M then the only assuranceis the size of the k.
         * @param a value to be compared with.
         * @param b value to be compared against.
         * @return share a>b e.g.1>=2=0
         * @exception NA
         * @todo
         <ul>
         <li>Use randomization to easily calculate the bit decomposition as in \cite Toft13.
         <li>Build Exceptions for field management.
         </ul>
         */
        Shares::StandardShare * lessThanToftShares(Shares::StandardShare * a, Shares::StandardShare * b);//Toft 2013
        
        //condition?true:false
        /**
         * @brief
         * Ternary operator that executes a secure conditional assigment (?true:false) process between 3 shares. If c is true returns a and b otherwise.
         * @details
         * Method calls the Util implementation of  multiplications.
         * @note this operation is not costless -2 communicational rounds needed
         * @param c boolean shared condition.
         * @param onTrue share to be returned in case true.
         * @param onFalse share to be returned in case false.
         * @return assingment if c:
         <ul>
            <li> 1=onTrue.
            <li> 0=onFalse.
         </ul>
         * @exception NA
         * @todo
         <ul>
            <li>Implement scalars equilent functions.
            <li>Build Exceptions for field management.
         </ul>
         */
        Shares::StandardShare * assigmentOperationShares(Shares::StandardShare *c, Shares::StandardShare *onTrue,Shares::StandardShare *onFalse);
        
        /**
         * @brief
         * Ternary operator that executes a secure conditional assigment (?true:false) process between 2 shares and a scalar. If c is true returns a and b otherwise.
         * @details
         * Method calls the Util implementation of  multiplications.
         * @note this operation is not costless -2 communicational rounds needed
         * @param c boolean shared condition.
         * @param onTrue share to be returned in case true.
         * @param onFalse share form to be returned in case false.
         * @return assingment if c:
         <ul>
         <li> 1=onTrue.
         <li> 0=onFalse.
         </ul>
         * @exception NA
         * @todo
         <ul>
         <li>Implement scalars equilent functions.
         <li>Build Exceptions for field management.
         </ul>
         */
        Shares::StandardShare * assigmentOperationShares(Shares::StandardShare *c, Shares::StandardShare *onTrue,long onFalse);
        
        /**
         * @brief Method use to securely swap standard shares.
         * @details Receive two shares and interchange their values.
         * @param playerId player who owns the share to be sent.
         * @param value  is the value of the share
         * @return share Configured as a share of playerId with value value.
         * @exception NA
         * @todo
         <ul>
         <li>Build Exceptions for non interpolable data
         </ul>
         */
         int exchangeShares(Shares::StandardShare * a, Shares::StandardShare *b);
        /**
         * @brief Method use to securely swap standard shares.
         * @details Receive two shares and interchange their values depending on the condition.
         * @param playerId player who owns the share to be sent.
         * @param value  is the value of the share
         * @return share Configured as a share of playerId with value value.
         * @exception NA
         * @todo
         <ul>
         <li>Build Exceptions for non interpolable data
         </ul>
         */
         int conditionalExchangeShares(Shares::StandardShare * c, Shares::StandardShare * a, Shares::StandardShare *b);
        
        //connectivity methods
        
        /**
         * @brief
         * Utilitarian Method builds Share Datatype using estate variables. These kind of shares that do not encrypt data are sometimes needed on some procedures.
         * @details
         * Its creation was motivated to use dummy shares with values like 0 and 1. All participants on the algortihm have to respect the value alocated for the share.
         * Can be easily reconstructed as well as all parties have the same value instantiated.
         * @param value numeric value to be wrapped on shared form.
         * @return share wrapped shared form of value
         * @exception NA
         * @todo
         <ul>
         <li> Eliminate the necissity of build share. Dynamize method selection so when no share operations needed can be replaced by scalars on all suitable calls of this method
         <li>Build Exceptions for field management.
         </ul>
         */
        Shares::StandardShare * buildShare(long value);
        
        //TODO: Add methods for greater than.. to save the greater one that way we eliminate the comparission
        
        /**
         * @brief
         * Sends a list of shares to all the players and in turn, players receiv a list of the lists of the shares representing all the submitted shares for reconstruction.
         * @details
         * Method ideally thought to be used for information verification. A selected group of shares is transmitted. All players do the same. and the result is a nested Utils::List.
         * @note this is a synchronic process, all the shares have to arrive
         * @param shares list of shares to be transmitted.
         * @return lists lists of shares corresponding to each share transmitted
         * @exception NA
         * @todo
         <ul>
         <li>Override method with custom player lists.
         <li>Build Exceptions for field management.
         </ul>
         */
        Utils::List<Utils::List<Shares::StandardShare> > * syncGlobal(Utils::List<Shares::StandardShare> * shares);
        
        /**
         * @brief
         * Transform a list of local shares into their corresponding open values.
         * @details
         * Uses transmission and sync methods to obtain and then reconstruct using our optimized version of lagrangian to open the shares. The function is similar to VIFF gather_shares method.
         * @note this is a synchronic process, all the shares have to arrive
         * @param shares list of shares to be open.
         * @return vector open values of the shares
         * @exception NA
         * @todo
         <ul>
         <li>Override method with custom player lists.
         <li>Build Exceptions for field management.
         <li> A possible alternative could be that all shares have a list of the players they have been shared with. That would allow to agilize the process but would also signify slower communications.
         </ul>
         */
        std::vector<long>  reconstructShares(Utils::List<Shares::StandardShare> * shares);
        
        /**
         * @brief
         * Transform a list of local shares into its corresponding open value.
         * @details
         * Uses transmission and sync methods to obtain and then reconstruct using our optimized version of lagrangian a share.
         * @note this is a synchronic process, all the shares have to arrive. The function is similar to VIFF gather_shares method.
         * @param share  single share to be open.
         * @return value open value of the shares
         * @exception NA
         * @todo
         <ul>
         <li>Override method with custom player lists.
         <li>Build Exceptions for field management.
         <li> A possible alternative could be that all shares have a list of the players they have been shared with. That would allow to agilize the process but would also signify slower communications.
         </ul>
         */
        long reconstructShare(Shares::StandardShare * share);

        /**
         * @brief
         * Transform a list of local shares into its corresponding open value and format it if signed.
         * @details
         * Uses transmission and sync methods to obtain and then reconstruct using our optimized version of lagrangian a share.
         * @note this is a synchronic process, all the shares have to arrive. The function is similar to VIFF gather_shares method.
         * @param share  single share to be open.
         * @return value open value of the shares
         * @exception NA
         * @todo
         <ul>
         <li>Override method with custom player lists.
         <li>Build Exceptions for field management.
         <li> A possible alternative could be that all shares have a list of the players they have been shared with. That would allow to agilize the process but would also signify slower communications.
         </ul>
         */
        long presentShare(Shares::StandardShare * share);
        
        //Transmission Methods
        
        /**
         * @brief
         * Sends a share to all the players and in turn, players receive a listthe shares representing the submitted share for reconstruction.
         * @details
         * Method ideally thought to be used for information verification. A selected group of shares is transmitted. All players do the same. and the result is a Utils::List.
         * @note this is a synchronic process, all the shares have to arrive
         * @param share  to be transmitted.
         * @return list  of shares corresponding to share transmitted
         * @exception NA
         * @todo
         <ul>
         <li>Override method with custom player lists.
         <li>Build Exceptions for field management.
         <li> Method was made generic to cover all scopes of transmision, (do everything method) should divide in small low coupled methods
         </ul>
         */
        Utils::List<Shares::StandardShare> * transmitShare(Shares::StandardShare *  share);
        
        /**
         * @brief
         * Decompose a value into shares and then transmits it to all players. Obtains the equivalents of all players in a collection Utils::List
         * @details
         * First generates Shamir Shares for the value, encapsulate the shares, then transmits them and wait for the correspondant of all players
         * @note this is a synchronic process, all the shares have to arrive
         * @param value to be shared.
         * @return list  of shares corresponding to values of all players.
         * @exception NA
         * @todo
         <ul>
         <li>Override method with custom player lists.
         <li>Build Exceptions for field management.
         <li>Build generic method to share on Share Manager that is capable to broadcast
         </ul>
         */
        Utils::List<Shares::StandardShare> * shareValue(long value);
        
        /**
         * @brief
         * Takes a Decomposed shared into its bitshares and then transmits them to all players. Obtains the equivalents of all players in a collection of  collections Utils::List for each bit transmitted
         * @details
         * Transmits them and wait for the correspondant of all players
         * @note this is a synchronic process, all the shares have to arrive. LSB is on the first position of the list.
         * @param shares bitwise shares to be transmitted representing a shared value.
         * @return list  lists of shares corresponding to the bit values of all players.
         * @exception NA
         * @todo
         <ul>
         <li>Override method with custom player lists.
         <li>Build Exceptions for field management.
         </ul>
         */
        Utils::List<Utils::List<Shares::StandardShare> > * transmitShareBitwise(Utils::List<Shares::StandardShare> * shares, int l);
        
        /**
         * @brief
         * Decompose a value into bitshares and then transmits them to all players. Obtains the equivalents of all players in a collection of  collections Utils::List for each bit transmitted
         * @details
         * First it decompose the number into bits, then, generates Shamir Shares for each bit value, encapsulate the shares, then transmits them and wait for the correspondant of all players
         * @note this is a synchronic process, all the shares have to arrive. LSB is on the first position of the list.
         * @param value to be shared bitwise.
         * @return list  lists of shares corresponding to the bit values of all players.
         * @exception NA
         * @todo
         <ul>
         <li>Override method with custom player lists.
         <li>Build Exceptions for field management.
         </ul>
         */
        Utils::List<Utils::List<Shares::StandardShare> > * shareValueBitwise(long value, int l);
        
        //custom destructor
        ~ShamirSharesEngine();
        //returns time on transmission
        double getTime();
       
    };
  
}

#endif
