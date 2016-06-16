//
//  ShamirSharesEngine.cpp
//  edgeRuntime
//  online modulus version branched
//  Any version of an Engine nees to instantiate th buffers and the ZZ field as well.
//  Created by Abdelrahaman Aly on 20/11/13.
//  Copyright (c) 2013 Abdelrahaman Aly. All rights reserved.
//

#include <iostream>

#include <math.h>
#include <NTL/ZZ.h>
#include <NTL/ZZ_p.h>
#include <NTL/ZZ_pX.h>

#include <cstdlib>
#include <mach/mach.h>
#include <mach/mach_time.h>

#include "MathUtil.h"

#include "StandardPlayer.h"
#include "StandardShare.h"

#include "SharesManager.h"
#include "ShamirSharesEngine.h"
#include "SharesListener.h"
#include "EngineBuffers.h"

#include "Constants.h"
#include "ShareUtil.h"
#include "List.h"


using namespace NTL;

namespace SmcEngines
{
    Listeners::SharesListener * ShamirSharesEngine::listener_=NULL;
       double ShamirSharesEngine::relativeTime=0;
    
    //Configuration Methods Implementations
    
    ShamirSharesEngine::ShamirSharesEngine(Players::StandardPlayer * player,  Utils::List<Players::StandardPlayer> * players, bool signed_)
    {
        //initialize engine varialbes
        this->player_ = player;
        this->players_=players;
        this->signed_=signed_;
        this->nBits_= Utilities::Constants::SYSTEM_B;
        this->p_=Utilities::Constants::SYSTEM_P;
        this->l_= Utilities::Constants::SYSTEM_L;
        this->k_=Utilities::Constants::SYSTEM_K;
        std::cout<<"p "<< this->p_<<" "<<sizeof(long long)<<"\n";
        NTL::ZZ_p::init(NTL::conv<NTL::ZZ>(this->p_));
        //std::cout<< "the result: "<< conv<ZZ_p>(2)*conv<ZZ_p>(2)<<"\n";
        //generates all alpha vectors that might be necessary to compare bit strings
        this->lAlphas_ = new vec_ZZ_p[this->nBits_];
        for (int i =1; i<= this->nBits_; i++)
        {
            this->lAlphas_[i-1]= VectorCopy(Utilities::MathUtil::multiplyLagrangePolynomials(1, i+1, 1),(i+1)+1);
        }
        
        //initialize context objects
        this->shareManager_ = new Managers::SharesManager(player, players);
        this->generator_= new ShareGenerators::ShamirGenerator(player,this->p_);
        
        //initialize application buffers
        for (int i=0;i<players_->getLength(); i++)
        {
            Buffers::EngineBuffers::syncBuffer_.push_back( NULL);
        }
        
        Buffers::EngineBuffers::workingBuffer_=new Utils::List<Shares::StandardShare>(this->players_->getLength());
        Buffers::EngineBuffers::nextOperationBuffer_=new Utils::List<Shares::StandardShare>(this->players_->getLength());
        
        //start listeners
        ShamirSharesEngine::listener_= new Listeners::SharesListener(this->player_,this->players_);
        ShamirSharesEngine::listener_->startListner();
    };
    
    Players::StandardPlayer * ShamirSharesEngine::getPlayer()
    {
        return this->player_;
    };
    //private native operations
    
    //Native implementation eliminating the conversion process should accelerate it. Loose of generality thinking on reliability.
    long ShamirSharesEngine::addSecure(long a, long b)
    {
        //Modulus operation perform by NTL libraries.
        NTL::ZZ_p ap= NTL::conv<NTL::ZZ_p>(a);
        ZZ_p bp= conv<ZZ_p>(b);
        return conv<long>(ap+bp);
    };
    
    //Native implementation eliminating the conversion process should accelerate it. Loose of generality thinking on reliability.
    long ShamirSharesEngine::multiplySecure(long a, long b)
    {
        //Modulus operation perform by NTL libraries.
        NTL::ZZ_p ap= NTL::conv<NTL::ZZ_p>(a);
        ZZ_p bp= conv<ZZ_p>(b);
        
        return conv<long>(ap*bp);
    };
    
    //add scalar with shared values
    long ShamirSharesEngine::addScalarSecure(long  a, long  shareValue)
    {
        return 0;
    };
    
    //returns  a-b in modulo p
    long ShamirSharesEngine::substractSecure(long  a, long  b)
    {
        //changes the sign of the share value -- this is just a -1 multiplication at the moment given that scalar mult.
        //can be done without communication.
        long aux = this->multiplySecure(-1, b);//multiplication method used bc it performs finite field mult.
        return this->addSecure(a, aux);// add method used bc it performs finit field add.
    };
    
    //private bitwise operations
    
    //calculates next carry bit
    Shares::StandardShare * ShamirSharesEngine::carrySharemindSecure(Shares::StandardShare * a, Shares::StandardShare * b,Shares::StandardShare * c)
    {
        Shares::StandardShare * carry = this->multiplyShares(a,b);
        Shares::StandardShare * aux= this->multiplyShares(b, c);
        carry= this->xorShares(carry, aux);
        aux= this->multiplyShares(a, c);
        carry=this->xorShares(carry, aux);
        
        delete aux;
        delete carry;
        
        return carry;
    };
    
    //performs bit addition in a secure fashion
    Utils::List<Shares::StandardShare> * ShamirSharesEngine::bitwiseAdditionSecure(Utils::List<Shares::StandardShare> * a,Utils::List<Shares::StandardShare> *b,Utils::List<Shares::StandardShare> * c)
    {
        int l= a->getLength();
        Utils::List<Shares::StandardShare> * sum= new Utils::List<Shares::StandardShare>(l);
        Shares::StandardShare * bit =NULL;
        for (int i=0; i<l-1; i++) {
            bit= this->multiply(c->get(i+1), -2);
            bit=this->addTo(bit, c->get(i));
            bit=this->addTo(bit, b->get(i));
            bit=this->addTo(bit, a->get(i));
            sum->add(bit);
        }
        bit=this->sxor(c->get(l-1), b->get(l-1));
        bit=this->xorTo(bit, a->get(l-1));
        sum->add(bit);
        return sum;
    };
    
    //transforms bit numbers to decimal numbers
    Shares::StandardShare * ShamirSharesEngine::btod(Utils::List<Shares::StandardShare> * bin, int size)
    {
        Shares::StandardShare * total = NULL;
        if (size>0)
        {
            Shares::StandardShare * aux =NULL;
            total= bin->get(0)->clone(); //It is equivalent to do bin[0]*2^0
            for (int i=1; i<size; i++)
            {
                aux=this->multiply(bin->get(i),  conv<long>(power(conv<ZZ>(2), i)));
                total= this->addTo(total, aux);
                delete aux;
            }
        }
        
        return total;
    };
    
    //generates a vector with a one signaling the most significative bit
    Utils::List<Shares::StandardShare> *  ShamirSharesEngine::obtainMostSignificativeBit(Utils::List<Shares::StandardShare> * c)
    {
        Shares::StandardShare * destroy = NULL;
        //dummy share of 1. It is a possibility to move it to the constants class.
        Shares::StandardShare * dummy = Utilities::ShareUtil::wrapStandardShare(this->player_->getPlayer(), 1);
        int l = c->getLength();//this->nBits_;
        
        Shares::StandardShare ** f= new Shares::StandardShare * [l];
        Shares::StandardShare ** d= new Shares::StandardShare * [l];
        
        Utils::List<Shares::StandardShare> * result = new Utils::List<Shares::StandardShare> (l);
        
        for (int i=l-1; i>=0; i--)// it should start from the l-1 bit, that is represented by the second one, the first one is array positioning related
        {
            Shares::StandardShare * aux= this->multiply(c->get(i), -1);
            aux=this->addTo(aux,dummy);
            if (i <= l-1-1) //the same as in the for, the number one correspond to the l-1 bit that is represented by the second one, the first one is array positioning related
                
            {
                //is the step 2 of the algorithm
                f[i]=this->multiply(f[i+1], aux);
                
                //is the step 3 of the algorithm
                destroy= aux;
                aux=this->multiply(f[i], -1);
                delete destroy;
                d[i]=(this->add(f[i+1], aux));
                
            }
            else
            {
                f[i]=(aux);
                aux=this->multiply(f[i], -1);
                d[i]=(this->add(dummy, aux));
            }
            delete aux;
        }
        //builds the array response
        for (int i =0; i< l; i++)
        {
            
            delete f[i];
            result->add(d[i]);
        }
        
        delete dummy;
        delete  []f;
        delete  []d;
        
        return result;
    };
    
    //calculates the complement 2 of a shared value given the bitsize desired for its transformation
    Utils::List<Shares::StandardShare> *  ShamirSharesEngine::obtainComplementTwoInShares(long value, int l=0)
    {
        if (l==0)
        {
            l=this->nBits_;
        }
        //int p= this->nBits_;
        int player=this->player_->getPlayer();
        int * iValues= NULL;
        iValues=Utilities::MathUtil::obtainComplementTwo(value, l);
        return Utilities::ShareUtil::wrapStandardShareList(player,  iValues, l);
    };
    
    //private comparison operations
    
    //Secure Equality and Greater-Than Tests with Sublinear Online Complexity.
    Shares::StandardShare * ShamirSharesEngine::greaterThanlBitsShares(Shares::StandardShare * a, Shares::StandardShare * b, int ap, int bp,int l)
    {
        Shares::StandardShare * destroy = NULL;
        //TODO:Check what has to be done on mod 2^l and why
        //--stopping condition of the recurssion
        if (l==1)
        {
            //1-[y]+[x][y]
            Shares::StandardShare * response= NULL;
            Shares::StandardShare * aux= this->substract((long)1, b);
            response =this->addTo(this->multiply(a, b),aux);
            delete aux;
            return response;
            
        }
        //--initialization of variables
        int k =this->k_;
        
        //operation constantly repeated around the formulation
        int ld2= l/2; //l/2
        ZZ_p lp2= power(conv<ZZ_p>(2), l); //2^(l)
        ZZ_p ld2p2= power(conv<ZZ_p>(2), ld2);//2^(l/2)
        
        //--[z]=2^l +x-y
        //puts in bit 2^l the operation x>= a given value
        Shares::StandardShare * z = this->substract(a, b);
        /*
         std::cout<<"l:"<< l<<" a: "<< this->reconstructShare(a)<<"\n";
         std::cout<<"l:"<< l<<" b: "<< this->reconstructShare(b)<<"\n";
         std::cout<<"l:"<< l<<" bz: "<< this->reconstructShare(z)<<"\n";
         */
        z=this->addTo(z, conv<long>(lp2));
        
        //--Preprocessing Random Number
        
        //Formulation to get a number of exactly 2^l+k random number with randombit generation for the l first bits.
        //R^(l) and r|_ - r-|
        Shares::StandardShare *r=NULL;
        Utils::List<Shares::StandardShare> * rBit= NULL;
        if( this->generateDagmardBitwiseRandomNumber(&(r),&(rBit),l)!= 1)
        {
            //TODO:implement error classes
            return NULL;
        }
        Shares::StandardShare * rTop=Utilities::ShareUtil::wrapStandardShare(this->player_->getPlayer(), 0);
        Shares::StandardShare  *rSub=Utilities::ShareUtil::wrapStandardShare(this->player_->getPlayer(), 0);
        //we use this form and not the provided methods by the framework because this form simplifies a for loop
        
        for(int i=0; i<ld2;i++)
        {
            ZZ_p twoCoeff= conv<ZZ_p>(2);
            twoCoeff=power(twoCoeff,i);
            
            destroy =this->multiply(rBit->get(i), conv<long> (twoCoeff));
            rSub=this-> addTo(rSub,destroy) ;
            delete destroy;
            
            destroy =this->multiply(rBit->get(i+ld2), conv<long> (twoCoeff));
            rTop=this-> addTo(rTop,destroy);
            delete destroy;
            
            
        }
        
        //Construction of R in such a way  the l less significative bits are equal to the r_l bits just generateed
        //Additionally R has to be a number of as top 2^l*k +l(n).
        //The only use of k is to give statistical certainty
        //r<z^k
        ZZ_p randK= conv<ZZ_p>((rand()% conv<long>(power(conv<ZZ_p>(2),k)-2))+1);//TODO: replace for shoup rand
        
        Utils::List<Shares::StandardShare> *randVals= this->shareValue(conv<long>(randK));
        
        
        Shares::StandardShare * R=this->multiplyTo(this->add(randVals->get(ap-1),randVals->get(bp-1)),conv<long>(power(conv<ZZ_p>(2),l)));// (2^l)*(r_a +r_b) <-> (2^l)*((2^(k))*2)
        
        //the multiplication by 2^l is like when you calculate the power of 10 to n value and just add the number of 0 to the right of the number
        //in there this is used to give back the ducenes (series of 2) that the first 2 bits have lost and then add the 2 less significative bits
        /*
         std::cout<<"l:"<< l<<" Ra: "<< this->reconstructShare(randVals->get(ap-1))<<"\n";
         std::cout<<"l:"<< l<<" Rb: "<< this->reconstructShare(randVals->get(bp-1))<<"\n";
         std::cout<<"l:"<< l<<" Rp: "<< this->reconstructShare(R)<<"\n";
         */
        R= this->addTo(R,r);// R_ + (2^l/2)*[r-|] +[r_|]--> (r)or this->add(this->multiply(rTop, conv<long>(power(conv<ZZ_p>(2),ld2))), rSub)
        
        //--online processing
        //randomization of  m
        Shares::StandardShare * mShare = this->add(z ,R);
        
        //open m and  put it inside the P field
        ZZ_p m = (conv<ZZ_p>(this->reconstructShare(mShare)));// reconstruct Share should return the value in the field but
        /*                                                       // for better reading we apply a conv
         //batch of debuggin
         std::cout<<"l:"<< l<<" m: "<< m<<"\n";
         std::cout<<"l:"<< l<<" r: "<< this->reconstructShare(r)<<"\n";
         std::cout<<"l:"<< l<<" z: "<< this->reconstructShare(z)<<"\n";
         std::cout<<"l:"<< l<<" rK: "<< randK<<"\n";
         
         std::cout<<"l:"<< l<<" rTop: "<< this->reconstructShare(rTop)<<"\n";
         std::cout<<"l:"<< l<<" rSub: "<< this->reconstructShare(rSub)<<"\n";
         std::cout<<"l:"<< l<<" R: "<< this->reconstructShare(R)<<"\n"; //
         */
        //separate  the top and the bottom bits on m
        //this operations are performed to be able to calculate z mod 2^l
        long lm =conv<long>(m);
        long mSub = lm% conv<long>(ld2p2);
        long mTop = (lm/conv<long>(ld2p2)) %conv<long>(ld2p2);// the division has the same effect as we were dividing  a decimal number
        // by the number of decimal you want to move to the right
        //std::cout<<"l:"<< l<<" mSub: "<< mSub<<"\n";
        //std::cout<<"l:"<< l<<" mTop: "<< mTop<<"\n";
        
        //obtains the equality from the top bits
        //TODO: implement an scalar  equality operation - look for possible improvements
        //TODO: manage the same naming standard example r and mShare.. should be rShare
        Shares::StandardShare * mTopShare = Utilities::ShareUtil::wrapStandardShare(this->player_->getPlayer(), mTop);
        Shares::StandardShare * e = this->equalToToftShares(mTopShare, rTop, l/2);// this variable is called b in Tofts paper -- it uses half of bits
        Shares::StandardShare *  mMin = this->addTo(this->multiply(e, conv<long>(conv<ZZ_p>(mSub)-conv<ZZ_p>(mTop))), mTop);
        
        Shares::StandardShare *  rMin = this->addTo(this->multiplyTo(this->substract(rSub, rTop),e), rTop);
        //recurssion
        Shares::StandardShare * smr=this->greaterThanlBitsShares(mMin, rMin, ap,bp,ld2);
        Shares::StandardShare * f = this->substract(1,smr);
        ZZ_p mMod=conv<ZZ_p>(lm% conv<long>(lp2));
        /*
         //info ethiquetes ->lTODO:ook for loggers in C++ and replace
         std::cout<<"l:"<< l<<"mMin: "<< this->reconstructShare(mMin)<<"\n"; //
         std::cout<<"l:"<< l<<"rMin: "<< this->reconstructShare(rMin)<<"\n"; //
         std::cout<<"l:"<< l<<"e: "<< this->reconstructShare(e)<<"\n"; //
         std::cout<<"l:"<< l<<"f: "<< this->reconstructShare(f)<<"\n"; //
         std::cout<<"l:"<< l<<"m: "<< lm<<"\n"; //
         std::cout<<"l:"<< l<<"m mod 2^l: "<< mMod<<"\n"; //
         */
        Shares::StandardShare *  zMod2l = this->substract(conv<long>(mMod), r);
        Shares::StandardShare * f2l=this->multiply(f, conv<long>(lp2));
        zMod2l= this->addTo(zMod2l,f2l);
        //std::cout<<"l:"<< l<<"zMod2l: "<< this->reconstructShare( zMod2l)<<"\n"; //
        //return zMod2l;
        ZZ_p zp=conv<ZZ_p>(power(conv<ZZ>(conv<ZZ_p>(1)/conv<ZZ_p>(2)),l));
        //std::cout<<"l:"<< l<<"zp: "<< zp<<"\n"; //
        Shares::StandardShare * rcontent=this->multiplyTo(this->substract(z, zMod2l),conv<long>(zp));//conv<long>(-1*lp2)
   
        
        Utilities::ShareUtil::destroyList(rBit);
        delete rSub;
        delete rTop;
        Utilities::ShareUtil::destroyList(randVals);
        delete R;
        delete z;
        delete mShare;
        delete mTopShare;
        delete e;
        delete mMin;
        delete rMin;
        delete smr;
        delete r;
        delete f;
        delete f2l;
        delete zMod2l;
        return rcontent;
        
    };
    
    //private transmission related methods
    
    //interpolates shared values
    long ShamirSharesEngine::interpolateValues(Utils::List<Shares::StandardShare> * shares)
    {
        int players = this->players_->getLength();
        long * values= new long[players];
        for (int i=0; i<players; i++)
        {
            values[i]= shares->get(i)->getValue();
        }
        long response =Utilities::MathUtil::lagrangianInterpolation(values, players);
        
        delete [] values;
        return response;
        
    };
    
    //Random number generation methods
    
    //generates a random number in a naive fashion.
    Shares::StandardShare * ShamirSharesEngine::generateShareRandomNumber()
    {
        return this->generateShareRandomNumber(this->p_);
    };
    
    //generates a random number in a naive fashion using custom thereshold
    Shares::StandardShare * ShamirSharesEngine::generateShareRandomNumber(long threshold)
    {
        int players = this->players_->getLength();
        int localRandom= (rand()% (threshold-2))+1;
        Utils::List<Shares::StandardShare> *  numberShares= this->shareValue(localRandom);
        Shares::StandardShare * number=NULL;
        if(players>0)
        {
            number= numberShares->get(0)->clone();
        }
        for (int i=1; i<players; i++)
        {
            number= this->addTo(number, numberShares->get(i));
        }
        
        Utilities::ShareUtil::destroyList(numberShares);
        
        return number;
    };
    
    //Uses Damgard technique to generate random bit
    Shares::StandardShare * ShamirSharesEngine::generateDagmardRandomBit()
    {
        ZZ_p aSquareValue;
        Shares::StandardShare * a=NULL;
        Shares::StandardShare * aSquare=NULL;
        do {
            delete a;
            delete aSquare;
            //obtain a random value
            a= this->generateShareRandomNumber();
            
            //square the random value
            aSquare= this->multiply(a, a);
            
            //Open the shares
            aSquareValue= conv<ZZ_p>(this->reconstructShare(aSquare));
            
        } while (aSquareValue==0);
        
        //get the square root of the value
        ZZ_p b= conv<ZZ_p>(SqrRootMod(conv<ZZ>(aSquareValue), conv<ZZ>(this->p_)));
        
        //get the inverse of the value
        b= conv<ZZ_p>(1)/b;//power(b,conv<ZZ>(-1));//
        
        //multiplication by the random number
        Shares::StandardShare * c= this->multiply(a,  conv<long>(b));
        
        //generates the bit excecuting (c+1)/2
        //obtain 1/2 or 2^-1
        ZZ_p oneHalf=power(conv<ZZ_p>(2),conv<ZZ>(-1));
        
        //execute final mathematical operations to convert -1;1 to 0;1
        Shares::StandardShare * d=this->add(c, 1);
        d=this->multiplyTo(d, conv<long>(oneHalf));
        
        delete a;
        delete aSquare;
        delete c;
        
        return d;
    };
    
    //generates random generated number using Damgard tehcniques.
    int ShamirSharesEngine::generateDagmardBitwiseRandomNumber(Shares::StandardShare ** number, Utils::List<Shares::StandardShare> ** bitwiseNumber, int l)
    {
        
        long p= this->p_;
        long openC=0;
        Utils::List<Shares::StandardShare> * rBitShares= NULL;
        
        do
        {
            Utilities::ShareUtil::destroyList(rBitShares);
            rBitShares= new Utils::List<Shares::StandardShare>(l);
            for(int i=0; i< l;i++)
            {

                rBitShares->add(this->generateDagmardRandomBit());

            };
            
            if(this->nBits_==l)
            {
                Shares::StandardShare * c = this->bitwiseLessThanSecureScalar(rBitShares, p);
                openC= this->reconstructShare(c);
                delete c;
            }
            else
            {
                openC=1;
            }
            
            
        }while (openC==0);
        
        Shares::StandardShare * r= obtainDecimalShareFromBits(rBitShares);
        
        *number=r;
        *bitwiseNumber=rBitShares;
        
        return 1;
    };
    
    // generate random nuber and its bit decomposition using a naive approach
    int ShamirSharesEngine::generateBitwiseRandomNumber(Shares::StandardShare ** number, Utils::List<Shares::StandardShare> ** bitwiseNumber, int l)
    {
        int players = this->players_->getLength();
        long p= this->p_;
        long localRandom= (rand()% p-1)+0;
        
        Utils::List<Shares::StandardShare> *  numberShares= this->shareValue(localRandom);
        Utils::List<Utils::List<Shares::StandardShare> >* bitShares= this->shareValueBitwise(localRandom,l);
        
        if(players>0)
        {
            *number= numberShares->get(0);
            *bitwiseNumber=bitShares->get(0)->clone();
        }
        Utils::List<Shares::StandardShare> * aux=NULL;
        for (int i=1; i<players; i++)
        {
            *number= this->addTo(*number, numberShares->get(i));
            aux = *bitwiseNumber;
            *bitwiseNumber=this->bitwiseAdditionShares(*bitwiseNumber, bitShares->get(i), Utilities::Constants::SHAREMIND_CARRY);
            
            Utilities::ShareUtil::destroyList(aux);
            Utilities::ShareUtil::destroyList(bitShares->get(i));
        }
        
        Utilities::ShareUtil::destroyList(numberShares);
        delete bitShares;
        
        return 1;
    };
    
    //Binary operation Methods
    
    //otains the decimal share form of a vector of shared bits
    Shares::StandardShare * ShamirSharesEngine::obtainDecimalShareFromBits(Utils::List<Shares::StandardShare> * a)
    {
        return this->btod(a, a->getLength());
    };
    
    //calculates the corresponding carry list of shared bits
    Utils::List<Shares::StandardShare> * ShamirSharesEngine::carrySharemindShares(Utils::List<Shares::StandardShare> * a, Utils::List<Shares::StandardShare> * b)
    {
        int l=a->getLength();
        Utils::List<Shares::StandardShare> * c= new Utils::List<Shares::StandardShare>(l);
        c->add(Utilities::ShareUtil::wrapStandardShare(this->player_->getPlayer(), 0));// it should be always a number modulus the correct  value
        //executes the operation for each pair of bits
        for (int i=0; i<l-1; i++)
        {
            c->add(this->carrySharemindSecure(a->get(i), b->get(i),c->get(i)));
        }
        return c;
        
    };
    
    //performs a bit addition using the method selected
    Utils::List<Shares::StandardShare> * ShamirSharesEngine::bitwiseAdditionShares(Utils::List<Shares::StandardShare> * a,Utils::List<Shares::StandardShare> * b,int type)
    {
        Utils::List<Shares::StandardShare> * carries= NULL;
        switch (type)
        {
            case 1: //Utilities::Constants::SHAREMIND_CARRY
                carries= this->carrySharemindShares(a, b);
                break;
                
            default:
                break;
        }
        return this->bitwiseAdditionSecure(a, b, carries);
        
    };
    
    //decompose a shared value into its bitshares - uses deafult bitsize for that matter, build a method that parametrize this option.
    Utils::List<Shares::StandardShare> *  ShamirSharesEngine::obtainBitsFromShare(Shares::StandardShare * share)
    {
        int l= this->nBits_;
        Shares::StandardShare * a= share;
        //p stands for pointer
        Utils::List<Shares::StandardShare> ** pRanBin =new Utils::List<Shares::StandardShare> *;
        Shares::StandardShare ** pRan= new Shares::StandardShare *;
        
        //fills the pointers of the random number
        if (this->generateBitwiseRandomNumber(pRan, pRanBin, l)!=1)
        {
            return NULL;
        }
        
        Shares::StandardShare * ran= *(pRan);
        Utils::List<Shares::StandardShare> * ranBin= *(pRanBin);
        Shares::StandardShare * nA= this->multiply(a, -1);
        Shares::StandardShare * c= this->add(ran, nA);
        
        //Open Share c
        long cValue= this->reconstructShare(c);
        
        Utils::List<Shares::StandardShare> * complementTwoCValue= this->obtainComplementTwoInShares(cValue, l);
        
        
        Utils::List<Shares::StandardShare> * d= this->bitwiseAdditionShares(ranBin, complementTwoCValue, Utilities::Constants::SHAREMIND_CARRY);
        //TODO: Complete the algorithm with the validation needed when it works on a given field
        
        delete a;
        Utilities::ShareUtil::destroyList( *pRanBin);
        delete *pRan;
        delete ran;
        delete ranBin;
        delete nA;
        delete c;
        Utilities::ShareUtil::destroyList(complementTwoCValue);
        
        return d;
    };
    
    // returns the result of a>b
    Shares::StandardShare * ShamirSharesEngine::bitwiseGreaterThanSecure(Utils::List<Shares::StandardShare> * a,Utils::List<Shares::StandardShare> * b)
    {
        int l= a->getLength();//this->nBits_;
        int player=this->player_->getPlayer();
        Utils::List<Shares::StandardShare> * c= new Utils::List<Shares::StandardShare> (l);
        Utils::List<Shares::StandardShare> * d=NULL;
        Utils::List<Shares::StandardShare> * e= new Utils::List<Shares::StandardShare> (l);
        Shares::StandardShare * result= Utilities::ShareUtil::wrapStandardShare(player, 0);
        //parallelism, this could be a parallel task for instance, to try to accelerate the performance
        for (int i=0; i<l; i++)
        {
            c->add(this->sxor(a->get(i), b->get(i)));
        }
        d=this->obtainMostSignificativeBit(c);
        for (int i=0; i<l; i++)
        {
            e->add(this->multiply(a->get(i), d->get(i)));
        }
        for (int i=0; i<l; i++)
        {
            result= this->addTo(result, e->get(i));
        }
        Utilities::ShareUtil::destroyList(c);
        Utilities::ShareUtil::destroyList(d);
        Utilities::ShareUtil::destroyList(e);
        return result;
    };
    
    //if a<b returns 1 else (a>=b) returns 0
    Shares::StandardShare * ShamirSharesEngine::bitwiseLessThanSecureScalar(Utils::List<Shares::StandardShare> * a,long b)
    {
        int bits= a->getLength();
        int player=this->player_->getPlayer();
        Utils::List<Shares::StandardShare> * c= new Utils::List<Shares::StandardShare> (bits);
        Utils::List<Shares::StandardShare> * d=NULL;
        Utils::List<Shares::StandardShare> * e= new Utils::List<Shares::StandardShare> (bits);
        Shares::StandardShare * result= Utilities::ShareUtil::wrapStandardShare(player, 0);
        int* bBits= Utilities::MathUtil::obtainBits(b, bits);
        //parallelism, this could be a parallel task for instance, to try to accelerate the performance
        for (int i=0; i<bits; i++)
        {
            c->add(this->sxor(a->get(i), bBits[i]));
        }
        
        d=this->obtainMostSignificativeBit(c);
        
        for (int i=0; i<bits; i++)
        {
            e->add(this->multiply( d->get(i),bBits[i]));
        }
        for (int i=0; i<bits; i++)
        {
            result= this->addTo(result, e->get(i));
        }
        
        Utilities::ShareUtil::destroyList(c);
        Utilities::ShareUtil::destroyList(d);
        Utilities::ShareUtil::destroyList(e);
        delete [] bBits;
        
        return result;
    };
    
    //if a== 0 returns 1 else (a>=b) returns 0
    Shares::StandardShare * ShamirSharesEngine::zeroTestXorShares(Shares::StandardShare * a)
    {
        int l= this->l_;
        int player=this->player_->getPlayer();
        Utils::List<Shares::StandardShare> * d =NULL;
        Utils::List<Shares::StandardShare> * e= new Utils::List<Shares::StandardShare> (l);

        Shares::StandardShare * R= this->generateShareRandomNumber(conv<long>(power(conv<ZZ_p>(2), this->k_-1 )));
        Shares::StandardShare *r=NULL;
        Utils::List<Shares::StandardShare> * rBit= NULL;
        if( this->generateDagmardBitwiseRandomNumber(&(r),&(rBit),l)!= 1)
        {
            //TODO:implement error classes
            return NULL;
        }
        //std::cout << "Operation after bit generation: "<<Buffers::EngineBuffers::operationCounter_<<"\n";
        
        Shares::StandardShare * c = this->add(a, conv<long>(power(conv<ZZ_p>(2),l-1)));
        Shares::StandardShare * aux = this->multiply(R,conv<long>(power(conv<ZZ_p>(2),l)));
        c= this->addTo(c,aux);
        c= this->addTo(c,r);
        long o_c= this->reconstructShare(c);
        o_c= o_c %conv<long>(power(conv<ZZ_p>(2),l));
        delete aux;
        
        int* bBits= Utilities::MathUtil::obtainBits(o_c, l);
        //parallelism, this could be a parallel task for instance, to try to accelerate the performance
        for (int i=0; i<l; i++)
        {
            e->add(this->sxor(rBit->get(i), bBits[i]));
        }
        d=this->obtainMostSignificativeBit(e);
        Shares::StandardShare * equal = Utilities::ShareUtil::wrapStandardShare(player, 0);
        for (int i=0; i<l; i++)
        {
            equal =this->addTo(equal,d->get(i));

        }
        equal = this->substractTo(1, equal);

        delete r;
        delete R;
        delete c;
        Utilities::ShareUtil::destroyList(rBit);
        Utilities::ShareUtil::destroyList(d);
        Utilities::ShareUtil::destroyList(e);
        delete [] bBits;
        
        return this->substractTo(1,equal);
    };
    
    
    Shares::StandardShare * ShamirSharesEngine::bitwiseLessThanSecureScalar(long b, Utils::List<Shares::StandardShare> * a)
    {
        Shares::StandardShare * c= this->bitwiseLessThanEqualSecureScalar(a, b);
        c= this->substractTo(1, c);
        return c;
    };

    //if a<b returns 1 else (a>=b) returns 0
    Shares::StandardShare * ShamirSharesEngine::bitwiseLessThanEqualSecureScalar(Utils::List<Shares::StandardShare> * a,long b)
    {
        int bits= a->getLength();
        int player=this->player_->getPlayer();
        Utils::List<Shares::StandardShare> * c= new Utils::List<Shares::StandardShare> (bits);
        Utils::List<Shares::StandardShare> * d=NULL;
        Utils::List<Shares::StandardShare> * e= new Utils::List<Shares::StandardShare> (bits);
        int* bBits= Utilities::MathUtil::obtainBits(b, bits);
        //parallelism, this could be a parallel task for instance, to try to accelerate the performance
        for (int i=0; i<bits; i++)
        {
            c->add(this->sxor(a->get(i), bBits[i]));
        }
        
        d=this->obtainMostSignificativeBit(c);
        
        for (int i=0; i<bits; i++)
        {
            e->add(this->multiply( d->get(i),bBits[i]));
        }
        Shares::StandardShare * equal= Utilities::ShareUtil::wrapStandardShare(player, 0);
        Shares::StandardShare * result = Utilities::ShareUtil::wrapStandardShare(player, 0);
        for (int i=0; i<bits; i++)
        {
            equal =this->addTo(equal,d->get(i));
            result= this->addTo(result, e->get(i));
        }
        equal = this->substractTo(1, equal);
        Utilities::ShareUtil::destroyList(c);
        Utilities::ShareUtil::destroyList(d);
        Utilities::ShareUtil::destroyList(e);
        delete [] bBits;
        Shares::StandardShare * aux = this->add(equal,result);
        Shares::StandardShare * aux_m = this->multiply(equal, result);
        delete result;
        aux= this->substractTo(aux, aux_m);
        result = aux;
        
        delete  equal;
        delete aux_m;

        return result;
    };

    
    //scalar public operations
    //*a+b where b is scalar
    Shares::StandardShare * ShamirSharesEngine::addScalar(Shares::StandardShare * a, long b)
    {
        return Utilities::ShareUtil::wrapStandardShare(this->player_->getPlayer(),this->addSecure( a->getValue(),b));
        
    };
    
    //a*b where b is scalar
    Shares::StandardShare * ShamirSharesEngine::multiplyScalar(Shares::StandardShare * a, long b)
    {
        
        return Utilities::ShareUtil::wrapStandardShare(this->player_->getPlayer(),multiplySecure(a->getValue(),b));
    };
    
    //*a xor b where b is scalar
    Shares::StandardShare * ShamirSharesEngine::xorScalar(Shares::StandardShare * a, long b)
    {
        Shares::StandardShare * resultXor= this->multiply(a,b);
        resultXor= this->multiplyTo(resultXor, -2);
        resultXor=this->addTo(resultXor, a);
        resultXor=this->addTo(resultXor, b);
        return resultXor;
    };
    
    //a-b where b is scalar
    Shares::StandardShare * ShamirSharesEngine::substractScalar(Shares::StandardShare * a, long b)
    {
        return Utilities::ShareUtil::wrapStandardShare(this->player_->getPlayer(), this->substractSecure(a->getValue(),b));
    };
    
    //a-b where a is scalar
    Shares::StandardShare * ShamirSharesEngine::substractScalar(long b, Shares::StandardShare * a)
    {
        return Utilities::ShareUtil::wrapStandardShare(this->player_->getPlayer(), this->substractSecure(b,a->getValue()));
    };
    
    //Basic Shares operation Methods
    
    //a+b
    Shares::StandardShare * ShamirSharesEngine::addShares(Shares::StandardShare *a,Shares::StandardShare *b)
    {
        //Native implementation eliminating the conversion process should accelerate it. Loose of generality thinking on reliability.
        return Utilities::ShareUtil::wrapStandardShare(this->player_->getPlayer(), this->addSecure(a->getValue(),b->getValue()));
    };
    
    //returns a-b
    Shares::StandardShare * ShamirSharesEngine::substractShares(Shares::StandardShare *a,Shares::StandardShare *b)
    {
        return Utilities::ShareUtil::wrapStandardShare(this->player_->getPlayer(), this->substractSecure(a->getValue(),b->getValue()));
    };
    
    //a*b
    Shares::StandardShare * ShamirSharesEngine::multiplyShares(Shares::StandardShare * a, Shares::StandardShare * b)
    {
        Utils::List<Shares::StandardShare>  *list;
        Shares::StandardShare *  product= Utilities::ShareUtil::wrapStandardShare(this->player_->getPlayer(), 0);
        
        long simpleProduct=this->multiplySecure(a->getValue(),b->getValue());
        list= this->shareValue(simpleProduct);
        this->generator_->multiplicationRegeneration(product, players_->getLength(), list);
        Utilities::ShareUtil::destroyList(list);
        return product;
        
    };
    
    //a^(-1)
    Shares::StandardShare * ShamirSharesEngine::invertShare(Shares::StandardShare *a)
    {
        //declaration of variables
        Shares::StandardShare * r=NULL;
        Shares::StandardShare * mShare=NULL;
        ZZ_p m;
        long lm;
        //loop to assure m is invertible i.e.(different from 0)
        do
        {
            delete r;
            delete mShare;
            //get a random number and multiplies it for a, and then opens the result
            r= this->generateShareRandomNumber();
            mShare=this->multiply(a, r);
            m =conv<ZZ_p>( this->reconstructShare(mShare));
            
        }while (m==0);
        //inverse of m calculation
        m= 1/m;
        lm=conv<long>(m);
        //simplification of the random factor, to get a clean and shared 1/a
        Shares::StandardShare * inverse= this->multiply(r, lm);
        
        delete r;
        delete mShare;
        
        return inverse;
    };
    
    //
    Shares::StandardShare * ShamirSharesEngine::mod2m(Shares::StandardShare * a, int k, int m)
    {
        //random bits of size l
        Shares::StandardShare * R= this->generateShareRandomNumber(conv<long>(power(conv<ZZ_p>(2),k +this->k_-1 -m)));
        Shares::StandardShare *r=NULL;
        Utils::List<Shares::StandardShare> * rBit= NULL;
        if( this->generateDagmardBitwiseRandomNumber(&(r),&(rBit),m)!= 1)
        {
            //TODO:implement error classes
            return NULL;
        }
        
        Shares::StandardShare * c = this->add(a, conv<long>(power(conv<ZZ_p>(2),k-1)));
        Shares::StandardShare * aux = this->multiply(R,conv<long>(power(conv<ZZ_p>(2),m)));
        c= this->addTo(c,aux);
        c= this->addTo(c,r);
        long o_c= this->reconstructShare(c);
        delete aux;
        o_c= o_c%conv<long>(power(conv<ZZ_p>(2),m));

        Shares::StandardShare * u=this->bitwiseLessThanSecureScalar(o_c,rBit);// r<=c :: c<r

        Shares::StandardShare * result =this->substract(o_c, r);
        aux= this->multiply(u, conv<long>(power(conv<ZZ_p>(2),m)));
        result = this->addTo(result, aux);
        
        delete R;
        Utilities::ShareUtil::destroyList(rBit);
        delete r;
        delete u;
        delete aux;
        delete c;
        return result;

    };
    
    //a^e the list contains only non zero powers. Power of 0 that is equal  to 1 is excluded from the list.
    Utils::List<Shares::StandardShare> * ShamirSharesEngine::powerShare(Shares::StandardShare * share, int e)
    {
        Utils::List<Shares::StandardShare> * list = new Utils::List<Shares::StandardShare>(e);
        
        //create elements for the fanInMultiplications
        for (int i=0; i<e; i++)
        {
            list->add(share);
        }
        Utils::List<Shares::StandardShare> * result =this->fanInMultiplicationShares(list);
        
        delete list; //is delete list and not destroyList because we do not want to delete the items inside the list,
        //(references to the same share) but  we want to eliminate its container
        
        return result;
    };
    
    //fan in operations
    Utils::List<Shares::StandardShare> * ShamirSharesEngine::fanInMultiplicationShares(Utils::List<Shares::StandardShare> * shares)
    {
        //this method works only with shared values and perform only secure operations.
        //which means if you know in advance the first element on this multiplication is going to be a 1 you could not use it.
        
        int size=shares->getLength();
        Utils::List<Shares::StandardShare> * list = new Utils::List<Shares::StandardShare>(size);
        list->add(shares->get(0)->clone());
        for(int i=1; i<size;i++)
        {
            list->add(this->multiply(list->get(i-1), shares->get(i)));
            
        }
        return list;
    };
    
    //secure methods: polymorphism
    
    //a+b
    Shares::StandardShare * ShamirSharesEngine::add(Shares::StandardShare *a,Shares::StandardShare *b)
    {
        //any normalization here
        return this->addShares(a, b);
    };
    
    //a+b
    Shares::StandardShare * ShamirSharesEngine::add(Shares::StandardShare * a, long b)
    {
        return this->addScalar(a, b);
    };
    
    //a+=b
    Shares::StandardShare * ShamirSharesEngine::addTo(Shares::StandardShare *a,Shares::StandardShare *b)
    {
        //any normalization here
        Shares::StandardShare *result =this->add(a, b);
        delete a;
        return result;
    };
    
    //a+=b
    Shares::StandardShare * ShamirSharesEngine::addTo(Shares::StandardShare *a,long b)
    {
        //any normalization here
        Shares::StandardShare *result =this->add(a, b);
        delete a;
        return result;
    };
    
    //a-=b
    Shares::StandardShare * ShamirSharesEngine::substractTo(Shares::StandardShare *a,Shares::StandardShare *b)
    {
        //any normalization here
        Shares::StandardShare *result =this->substract(a, b);
        delete a;
        return result;
    };
    
    //a-=b
    Shares::StandardShare * ShamirSharesEngine::substractTo(Shares::StandardShare *a, long b)
    {
        //any normalization here
        Shares::StandardShare *result =this->substract(a, b);
        delete a;
        return result;
    };
    
    //a-=b
    Shares::StandardShare * ShamirSharesEngine::substractTo( long b, Shares::StandardShare *a )
    {
        //any normalization here
        Shares::StandardShare *result =this->substract(b,a);
        delete a;
        return result;
    };
    
    //a-b
    Shares::StandardShare * ShamirSharesEngine::substract(Shares::StandardShare *a,Shares::StandardShare *b)
    {
        //any normalization here
        return this->substractShares(a, b);
    };
    
    //a-b
    Shares::StandardShare * ShamirSharesEngine::substract(Shares::StandardShare * a, long b)
    {
        //any normalization here
        return this->substractScalar(a, b);
        
    };
    
    //a-b
    Shares::StandardShare * ShamirSharesEngine::substract( long b, Shares::StandardShare * a)
    {
        //any normalization here
        return this->substractScalar(b, a);
        
    };
    
    //a*=b
    Shares::StandardShare * ShamirSharesEngine::multiplyTo( Shares::StandardShare * a, Shares::StandardShare *b )
    {
        //any normalization here
        Shares::StandardShare *result =this->multiply(a, b);
        delete a;
        return result;
    };
    
    //a*=b
    Shares::StandardShare * ShamirSharesEngine::multiplyTo( Shares::StandardShare * a, long b )
    {
        //any normalization here
        Shares::StandardShare *result =this->multiply(a, b);
        delete a;
        return result;
    };
    
    //a*b
    Shares::StandardShare * ShamirSharesEngine::multiply(Shares::StandardShare *a,Shares::StandardShare *b)
    {
        //any normalization here
        return this->multiplyShares(a, b);
    };
    
    //a*b
    Shares::StandardShare * ShamirSharesEngine::multiply(Shares::StandardShare * a, long b)
    {
        return this->multiplyScalar(a, b);
    };
    
    //c= a xor b
    Shares::StandardShare * ShamirSharesEngine::sxor( Shares::StandardShare * a, Shares::StandardShare *b )
    {
        //any normalization here
        Shares::StandardShare *result =this->xorShares(a, b);
        return result;
    };
    
    //c=a xor b
    Shares::StandardShare * ShamirSharesEngine::sxor( Shares::StandardShare * a, long b )
    {
        //any normalization here
        Shares::StandardShare *result =this->xorScalar(a, b);
        return result;
    };
    
    //a xor=b
    Shares::StandardShare * ShamirSharesEngine::xorTo( Shares::StandardShare * a, Shares::StandardShare *b )
    {
        //any normalization here
        Shares::StandardShare *result =this->xorShares(a, b);
        delete a;
        return result;
    };
    
    //a xor=b
    Shares::StandardShare * ShamirSharesEngine::xorTo( Shares::StandardShare * a, bool b )
    {
        //any normalization here
        Shares::StandardShare *result =this->xorScalar(a, b);
        delete a;
        return result;
    };
    
    
    //public applications
    
    //a xor b
    Shares::StandardShare * ShamirSharesEngine::xorShares(Shares::StandardShare * a, Shares::StandardShare * b)
    {
        Shares::StandardShare * resultXor= this->multiply(a,b);
        resultXor= this->multiplyTo(resultXor, -2);
        resultXor=this->addTo(resultXor, a);
        resultXor=this->addTo(resultXor, b);
        return resultXor;
        
    };
    
    //a>b using bitwise decomposition -Damgar 2006 method
    Shares::StandardShare * ShamirSharesEngine::greaterThanShares(Shares::StandardShare * a, Shares::StandardShare * b)
    {
       // if (a->getBits()==NULL)
       // {
       //     a->setBits(this->obtainBitsFromShare(a));
       // }
       // if(b->getBits()==NULL)
        //{
        //    b->setBits(this->obtainBitsFromShare(b));
        //}
        return NULL;
        //return this->bitwiseGreaterThanSecure(a->getBits(), b->getBits());
    };
    
    //a==0
    Shares::StandardShare * ShamirSharesEngine::zeroTestShares(Shares::StandardShare * a,  int l)
    {
        /*
         //this method follows Toft's implementation on:
         //Secure Equality and Greater-Than Tests with Sublinear Online Complexity.
         //It strictly follows and uses the methods the original paper suggests.
         //Including all the Damgard[6] reference on Toft's paper for inverse random numbers
         //and random generation numbers.
         */
        
        Shares::StandardShare * rAdjustment = NULL;
        int k = this->k_;
        
        if (l!= this->nBits_)
        {
            //r_a= r(2^k +ln(n))*2^l
            rAdjustment=this->multiplyTo(this->generateShareRandomNumber(conv<long>(power(conv<ZZ_p>(2),k))), conv<long>(power(conv<ZZ_p>(2),l))); //once used destroys generated number
        }
        else
        {
            //r_a=0
            rAdjustment= Utilities::ShareUtil::wrapStandardShare(this->player_->getPlayer(), 0);
        }
        //gets a random number with 2^l ceroes bits
        //----preprocessing----
        int l_m = l;
        int player= this->player_->getPlayer();
        
        //generation r and its bit decomposition --using damgard bit decomposition and random number generation
        
        Utils::List<Shares::StandardShare> *rBits= NULL;
        Shares::StandardShare * r = NULL;
        
        if(this->generateDagmardBitwiseRandomNumber(&(r), &(rBits), l)!=1)
        {
            return NULL;
        }
        
        //generation of R and R inverse and it's potences using Damgard random number generation and fan in multiplication.
        Shares::StandardShare * R= this->generateShareRandomNumber();
        Shares::StandardShare * RInverse= this->invertShare(R);
        Utils::List<Shares::StandardShare> *RPowers=this->powerShare(R, l_m);
        
        
        // ----online phase----
        //calculate m and opens it
        Shares::StandardShare * mShare= this->addTo( this->add(a, r),rAdjustment); //once used the generated instance for addition it destroys it.
        
        ZZ_p m= conv<ZZ_p>( this->reconstructShare(mShare));
        
        int * mBits= Utilities::MathUtil::obtainBits(conv<long>(m), l_m);
        //calulates H+1. Sums xor of r bits and m bits
        // gets the sumatory of xor r vsm bits --Calculates Hamming distance H
        Shares::StandardShare * H= Utilities::ShareUtil::wrapStandardShare(player, 0);
        for (int i=0; i<l_m; i++)
        {
            Shares::StandardShare * lXor= this->sxor(rBits->get(i), mBits[i]);
            H= this->addTo(H,lXor );
            delete lXor;
        }

        //add one to hamming distance
        Shares::StandardShare * HPlusOne= this->add(H, 1);
        
        //randomize H+1 with R^-1
        Shares::StandardShare * m_hShare= this->multiply(RInverse, HPlusOne);
        ZZ_p m_h = conv<ZZ_p>(this->reconstructShare(m_hShare));
      
      
        //simplification of the random number  on the m and obtention of the m^i factors


        ZZ_p auxm=conv<ZZ_p>(1);
        Utils::List<Shares::StandardShare> *HPlusOnePowers= new Utils::List<Shares::StandardShare>(l_m+1);
        

        HPlusOnePowers->add(Utilities::ShareUtil::wrapStandardShare(player, 1));
        for (int i=1; i<=l_m; i++) {
            auxm=auxm*m_h;
            HPlusOnePowers->add(this->multiply(RPowers->get(i-1), conv<long>(auxm)));
            
        }
   
        //lagrange interpolation of the funciont P(x) where P(1)=1 and P(x)= 0 for x!=1 and 0< x<=m+1
        vec_ZZ_p alphas =  this->lAlphas_[l-1]; //VectorCopy(Utilities::MathUtil::multiplyLagrangePolynomials(1, l_m+1, 1),(l_m+1)+1); // this should be precalculated to accelerate the processing
        Shares::StandardShare * eq= this->multiply(HPlusOnePowers->get(0), conv<long>(alphas[0]));// in this case the first term should always be one
        for (int i=1; i<=l_m; i++)
        {
            Shares::StandardShare * destroy=this->multiply(HPlusOnePowers->get(i), conv<long>(alphas[i]));
            eq= this->addTo(eq, destroy);
            delete destroy;
        }
  
        delete rAdjustment;
        Utilities::ShareUtil::destroyList(rBits);
        delete  r;
        Utilities::ShareUtil::destroyList(RPowers);
        delete R;
        delete RInverse;
        delete mShare;
        delete  [] mBits;
        delete H;
        delete HPlusOne;
        delete m_hShare;
        Utilities::ShareUtil::destroyList(HPlusOnePowers);
       // delete eq;
        return eq;

    };
    
    //returns a-b=0
    Shares::StandardShare * ShamirSharesEngine::equalToToftShares(Shares::StandardShare * a, Shares::StandardShare * b, int l)
    {
        totalCom++;
        //stopping condition
        if (l==1)
        {
            Shares::StandardShare * rXor =this->sxor(a, b);
            Shares::StandardShare * result=this->substract((long)1,rXor);
            delete rXor;
            return result;// x=1- a xor b if a =1, x=0 - a=0, x=1
        }
        else if (l==0)
        {
            l=this->nBits_;
        }
        
        //instead of a secure operation we perform a mod addition on the bit size
        //x= Utilities::ShareUtil::wrapStandardShare(this->player_->getPlayer(), conv<long>( AddMod(a->getValue(), x->getValue(), l)));
        //x = Utilities::ShareUtil::wrapStandardShare(this->player_->getPlayer(), conv<long>(SubMod(SubMod(a->getValue(), b->getValue(),conv<long>(aux)),conv<long>(aux),conv<long>(aux))));
        
        //calculate the difference between a and b
        Shares::StandardShare * x= this->substract(a, b);
        //long aux= conv<long>(power(conv<ZZ>(2),l));
        
        //Shares::StandardShare * R = this->generateShareRandomNumber(); //the game of multiplying by the random and then
        //Shares::StandardShare * invR= this->invertShare(R);            //doing the operation and multiplying by the inverse
        //is not possible, given that x%2^l is not equvalent to ((r*x)%2^l)/r
        //Shares::StandardShare * xo= x;
        //x= multiply(R, x);
        //ZZ_p xp =conv<ZZ_p>(this->reconstructShare(x)/ aux);        //the game of multiplying by the random and then
        //x= this->multiply(invR, conv<long>(xp));                    //doing the operation and multiplying by the inverse
        //x=this->substract(xo, x);                                   //is not possible, given that x%2^l is not equvalent to floor(((r*x)/2^l))/r
        //std::cout<<"l:"<<l<<"a: "<< this->reconstructShare(a)<< " b: " << this->reconstructShare(b)<<" xp: "<< xp<< " xo: "<<this->reconstructShare(xo)<<" x: "<<this->reconstructShare(x)<<" R: "<< this->reconstructShare(R)<< " RI: "<< this->reconstructShare(invR)<<"\n";
        //check whether the difference is equal to 0
        Shares::StandardShare * result=this->zeroTestShares(x,l);
        
        delete x;
        
        return result;
        
    };
    
    //encapsulates access to nBits_ fro comparison procedure
    Shares::StandardShare * ShamirSharesEngine::equalToToftShares(Shares::StandardShare * a, Shares::StandardShare * b)
    {
        return this->equalToToftShares(a, b,this->nBits_);
    };
    
    //returns a>=b -memory save method
    Shares::StandardShare * ShamirSharesEngine::greaterEqualThanToftShares(Shares::StandardShare * a, Shares::StandardShare * b)
    {
        int pa=a->getPlayerId();
        int pb= b->getPlayerId();
        //in case of use of build function for shares the difference between the players has to be stated
        if(pa==pb)
        {
            pa=1; //assumtion first player
            pb=2; //assumption second player
        }
        Shares::StandardShare * response =NULL;
        long p2l=conv<long>(power(conv<ZZ_p>(2), this->l_/2)-1);
        if(this->signed_==true)
        {
            Shares::StandardShare *a_aux= this->add(a, p2l);
            Shares::StandardShare *b_aux= this->add(b, p2l);

            response=this->greaterThanlBitsShares(a_aux, b_aux,pa,pb, this->l_);
            delete a_aux;
            delete b_aux;
        }
        else
        {
            response= this->greaterThanlBitsShares(a, b,pa,pb, this->l_);
        }
        //this method receives the players as parameters fiven that they are necessary to select the secret random numbers - this is to extriclty adhere to the algorithm
        return response;
    };
    
    
    
    //This is actually  Catrinas method for comparitions. Implementation Memory Secure
    Shares::StandardShare * ShamirSharesEngine::lessThanZeroCatrinaModShares(Shares::StandardShare * a)
    {
        totalCom++;
        Shares::StandardShare * c= a->clone();
        long p2l2= conv<long>(power(conv<ZZ_p>(2),this->l_-1));
        
        long inv_p2l2= conv<long>(power(conv<ZZ_p>(p2l2),-1));

        Shares::StandardShare * res = this->mod2m(c, this->l_, this->l_-1);
        c= this->substractTo(c, res);
        Shares::StandardShare * result = this->multiplyTo(this->multiplyTo(c, inv_p2l2),-1);
        // is minus one because the division is being performed out of the field c is negative
        
        delete res;
        
        return result;
    };

    //ltz(a) :a<0
    Shares::StandardShare * ShamirSharesEngine::lessThanCatrinaModShares(Shares::StandardShare * a, Shares::StandardShare * b)
    {
        Shares::StandardShare * c = this->substract(a, b);
        Shares::StandardShare * result = this->lessThanZeroCatrinaModShares(c);
        delete c;
        return result;
    };
    
    //ltz(-a) : a>0
    Shares::StandardShare * ShamirSharesEngine::greaterThanCatrinaModShares(Shares::StandardShare * a, Shares::StandardShare * b)
    {
        Shares::StandardShare * c = this->substract(b, a);
        Shares::StandardShare * result = this->lessThanZeroCatrinaModShares(c);
        delete c;
        return result;
    };
    
    //1-ltz(-a) : a<=b
    Shares::StandardShare * ShamirSharesEngine::lessEqualThanCatrinaModShares(Shares::StandardShare * a, Shares::StandardShare * b)
    {
        Shares::StandardShare * c = this->substract(b, a);
        Shares::StandardShare * result = this->lessThanZeroCatrinaModShares(c);
        result= this->substractTo(1, result);
        delete c;
        return result;
    };
    //1-ltz(a) : a>=b
    Shares::StandardShare * ShamirSharesEngine::greaterEqualThanCatrinaModShares(Shares::StandardShare * a, Shares::StandardShare * b)
    {
        Shares::StandardShare * c = this->substract(a, b);
        Shares::StandardShare * result = this->lessThanZeroCatrinaModShares(c);
        result= this->substractTo(1, result);
        delete c;
        return result;
    };
    
    //c?onTrue:onFalse
    Shares::StandardShare * ShamirSharesEngine::assigmentOperationShares(Shares::StandardShare *c, Shares::StandardShare *onTrue,Shares::StandardShare *onFalse)
    {
        Shares::StandardShare * result =NULL;
        
        result= this->multiplyTo(this->substract(onTrue,onFalse), c);
        result= this->addTo(result,onFalse);
        return result;
    };
    
    Shares::StandardShare * ShamirSharesEngine::assigmentOperationShares(Shares::StandardShare *c, Shares::StandardShare *onTrue,long onFalse)
    {
        Shares::StandardShare * result =NULL;
        
        result= this->multiplyTo(this->substract(onTrue,onFalse), c);
        result= this->addTo(result,onFalse);
        return result;
    };
    
    //connectivity methods
    Shares::StandardShare * ShamirSharesEngine::buildShare(long value)
    {
        return Utilities::ShareUtil::wrapStandardShare(this->player_->getPlayer(), value);
    };
    
    //sync several shares across all players.
    Utils::List<Utils::List<Shares::StandardShare> > * ShamirSharesEngine::syncGlobal(Utils::List<Shares::StandardShare> * shares)
    {
        Utils::List<Utils::List<Shares::StandardShare> > *list= new Utils::List<Utils::List<Shares::StandardShare> >(players_->getLength());
        for (int i=0; i<shares->getLength(); i++)
        {
            Utils::List<Shares::StandardShare> * aux=this->transmitShare(shares->get(i));
            list->add(aux);
        }
        return list;
        
    };
    
    //returns a<b -memory save method
    Shares::StandardShare * ShamirSharesEngine::lessThanToftShares(Shares::StandardShare * a, Shares::StandardShare * b)
    {
        Shares::StandardShare * comp =this->greaterEqualThanToftShares(a, b);
        Shares::StandardShare * result =this->substract(1,comp);
        delete comp;
        return result;
    };
    
    
    //reconstruct several shares - similar to the method gather_shares in VIFF
    std::vector<long>  ShamirSharesEngine::reconstructShares(Utils::List<Shares::StandardShare> * shares)
    {
        int size = shares->getLength();
        std::vector<long> values;
        values.resize(size);
        Utils::List<Utils::List<Shares::StandardShare> > * syncShares= this->syncGlobal(shares);
        long ld2=0;
        if (signed_==true)
        {
            ld2= conv<long>(power(conv<ZZ_p>(2), this->l_-1));
        }
        for (int i=0; i<size; i++)
        {
            values[i]= this->interpolateValues(syncShares->get(i));
            Utilities::ShareUtil::destroyList(syncShares->get(i));
            if(signed_== true && values[i]>=ld2)
            {
                values[i]=values[i]- Utilities::Constants::SYSTEM_P;
            }
        }
        delete syncShares;
        
        
        return values;
    };
    
    //reconstruct single share
    long ShamirSharesEngine::reconstructShare(Shares::StandardShare * share)
    {
        
        Utils::List<Shares::StandardShare> * shares= this->transmitShare(share);
        long result=this->interpolateValues(shares);

        Utilities::ShareUtil::destroyList(shares);
        return result;
        
    };
    
    //present single share
    long ShamirSharesEngine::presentShare(Shares::StandardShare * share)
    {
        long result = this->reconstructShare(share);
        long lp2= conv<long>(power(conv<ZZ_p>(2),31));
        if(this->signed_ && result> lp2-1)
        {
            result =result - Utilities::Constants::SYSTEM_P;
        }
        return result;
    };
    //Tranmission Methods Implementations
    
    //Broadcast a share towards all the players, method used for sync, and reconstructions
    Utils::List<Shares::StandardShare> * ShamirSharesEngine::transmitShare(Shares::StandardShare *  share)
    {
        double begin_time = mach_absolute_time();

        //player claims ownership of the share before transmission
        Shares::StandardShare * localShare= share->clone();
        localShare->setPlayerId(this->player_->getPlayer());
        
        //create distribution list for  all players for everybody to be able to interpolate later
        Utils::List<Shares::StandardShare> *list = new Utils::List<Shares::StandardShare>(players_->getLength());
        for (int i=0; i<this->players_->getLength(); i++) {
            list->add(localShare);
        }
        //transimssion process
        Utils::List<Shares::StandardShare> * aux =this->shareManager_->transmitShares(list,  Buffers::EngineBuffers::operationCounter_+1);
        Buffers::EngineBuffers::operationCounter_++;
        
        //instead of using destroy list, destroy the shares individually that way we dont waste a for loop
        delete  localShare;
        delete list;

        
        ShamirSharesEngine::relativeTime += double( mach_absolute_time() - begin_time );
        
        return aux;
    };
    
    //method used to share original value, not to bradcast shares
    Utils::List<Shares::StandardShare> * ShamirSharesEngine::shareValue(long value)
    {
        double tInitial =mach_absolute_time();
        
        Utils::List<Shares::StandardShare> *list = new Utils::List<Shares::StandardShare>(players_->getLength());
        this->generator_->generateShares(value, players_->getLength(),list);
        //it invokes it directly because saving RAM memory, given that no modification is needed. i
        Utils::List<Shares::StandardShare> * aux =this->shareManager_->transmitShares(list,  Buffers::EngineBuffers::operationCounter_+1);
        Buffers::EngineBuffers::operationCounter_++;
        
        Utilities::ShareUtil::destroyList(list);
    
        ShamirSharesEngine::relativeTime += (mach_absolute_time() - tInitial);
        
        return aux;
        
    };
    
    // this method provides a matrix on which the columns are players and rows are the numbers.
    //for that matter we need to do a transposition of the natural disposition of the values
    Utils::List<Utils::List<Shares::StandardShare> > * ShamirSharesEngine::transmitShareBitwise(Utils::List<Shares::StandardShare> * shares, int l)
    {
        //ET: here change shares->getLength()
        int players= this->players_->getLength();
        Utils::List<Utils::List<Shares::StandardShare> > * list = new Utils::List<Utils::List<Shares::StandardShare> >(players);
        for(int i=0; i<players;i++)
        {
            list->add(new Utils::List<Shares::StandardShare>);
        }
        for (int i=0; i<l; i++)
        {
            Utils::List<Shares::StandardShare> * localList=this->transmitShare(shares->get(i));
            
            for (int j=0; j<players; j++)
            {
                list->get(j)->add(localList->get(j));
            }
            //list->add(this->transmitShare(shares->get(i)));
        }
        return list;
    };
    
    //this method provides a matrix on which the columns are players and rows are the numbers.
    //for that matter we need to do a transposition of the natural disposition of the values
    Utils::List<Utils::List<Shares::StandardShare> > * ShamirSharesEngine::shareValueBitwise(long value, int l)
    {
        //int size =this->nBits_;
        int players= this->players_->getLength();
        int * bits= Utilities::MathUtil::obtainBits(value, l);
        Utils::List<Utils::List<Shares::StandardShare> > * list = new Utils::List<Utils::List<Shares::StandardShare> >(players);
        
        //instantation of the lists
        for(int i=0; i<players;i++)
        {
            list->add(new Utils::List<Shares::StandardShare>);
        }
        
        //creation of the matrix of values
        for (int i=0; i<l; i++)
        {
            Utils::List<Shares::StandardShare> * localList=this->shareValue(bits[i]);
            for (int j=0; j<players; j++)
            {
                list->get(j)->add(localList->get(j));
            }
            
        }
        
        delete [] bits;
        
        return list;
    };
    
    
    ShamirSharesEngine::~ShamirSharesEngine()
    {
        listener_->stopListener();
        delete Buffers::EngineBuffers::nextOperationBuffer_;
        delete Buffers::EngineBuffers::workingBuffer_;
        delete this->listener_;
        delete this->shareManager_;
        delete this->generator_;
        
        for (int i=0; i<Buffers::EngineBuffers::incomingSharesBuffer_.capacity(); i++)
        {
            
            Shares::StandardShare *aux=Buffers::EngineBuffers::incomingSharesBuffer_[i];
            if (aux!=NULL) {
                
                delete aux;
            }
            Buffers::EngineBuffers::incomingSharesBuffer_.clear();
            
        }
    };
    
    double    ShamirSharesEngine::getTime()
    {
        //return 1;
        return ShamirSharesEngine::relativeTime;
    };
}