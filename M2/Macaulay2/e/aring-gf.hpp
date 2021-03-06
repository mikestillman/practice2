// Copyright 2011 Michael E. Stillman

#ifndef _aring_gf_hpp_
#define _aring_gf_hpp_

#include "aring.hpp"
#include "buffer.hpp"
#include "ringelem.hpp"
#include <iostream>


//enable this lines to trick Kdevelop
// #define HAVE_FFLAS_FFPACK 1 
// #define HAVE_GIVARO 1

#include "polyring.hpp"
class RingMap;



#if not   defined(HAVE_GIVARO)

#include "aring-m2-gf.hpp"

namespace M2 {

   
   class ARingGF : public DummyRing
   //class ARingGF : public ARingGFM2
   {
    public:
        static const RingID ringID = ring_GF;

        typedef M2::ARingGF             ring_type ;
     
        ARingGF( int charac_,   int dimension_)  {};
        ARingGF( int charac_,  
           const M2_arrayint & modPolynomial, 
           const PolynomialRing &originalR
           )  {}
        ARingGF( int charac_,  
           const M2_arrayint & modPolynomial, 
           const M2_arrayint & primitiveElement, 
           const PolynomialRing &originalR
           )  {}
   };
};

#else
#include <givaro/givgfq.h>
#include <givaro/givpower.h>
#include <givaro/givtimer.h>
#include <givaro/givextension.h>     //multiple definition problem...   solvable by encapsulating (see linbox)? Also solvable with the namespace trick, but do not overuse that...
#include <math.h>
#include <givaro/givinteger.h>
#include <givaro/givintnumtheo.h>
#include <givaro/givpower.h>
#include <givaro/givpoly1padic.h>




namespace M2 {

/**
    @ingroup rings

    @brief wrapper for the  Givaro::GFqDom<>  galois field implementation
*/
/// @todo: think about deriving from RingInterface AND from Ring
class ARingGF : public RingInterface
{

  public:
    static const RingID ringID = ring_GF;

    typedef Givaro::GFqDom<long>    FieldType;
    typedef FieldType::Element      ElementType;
    typedef M2::ARingGF             ring_type ;
  
    typedef ElementType     elem;

    typedef  FieldType::Residu_t     UTT; ///< types depends on FieldType definition!
    typedef Signed_Trait<FieldType::Residu_t>::signed_type  STT;///< types depends on FieldType definition!


    ARingGF( UTT charac_,   UTT dimension_);

  // returns a polynomial that Givaro would choose for this GF(mCharac^dim).
  // We hope that if the polynomial is F(t), that t is a generator of the
  // multiplicative group.  We need to check this.
  //TODO: check whether Givaro can handle F(t) with t not primitive.
  static const M2_arrayint findMinimalPolynomial(UTT charac, UTT dim);

  ARingGF( UTT charac_,  
           const M2_arrayint & modPolynomial, 
           const PolynomialRing &originalR
           //TODO: other information too?
           );

  ARingGF( UTT charac_,  
           const M2_arrayint & modPolynomial,
            const M2_arrayint & generatorPoly,  
           const PolynomialRing &originalR
           //TODO: other information too?
           );

  const FieldType field() const {return givaroField;}

  private:
   
    UTT     mCharac;
    UTT     mDimension; ///< same as extensionDegree

    const PolynomialRing*   mOriginalRing;
    const ring_elem         mPrimitiveElement; // is an element of mOriginalRing

    const FieldType     givaroField;
 
    mutable  FieldType::randIter     givaroRandomIterator;

    size_t      mGeneratorExponent;  

    //  int p1; // p-1
    // int minus_one;
    // int prim_root; // element we will use for our primitive root


    M2_arrayint     representationToM2Array(UTT representation,  long coeffNum ) const;

    static M2_arrayint     representationToM2Array(UTT representation,  long coeffNum, UTT charac ) ;

    M2_arrayint     modPolynomialRepresentationToM2Array(UTT representation) const;
    M2_arrayint     elementRepresentationToM2Array(UTT representation) const;
    


  public:
    M2_arrayint fieldElementToM2Array(ElementType el) const;

  private:
    static      UTT               M2arrayToGFRepresentation(UTT pCharac , const  M2_arrayint & m2array ) ;
    static      std::vector<UTT>  M2arrayToStdVec(UTT pCharac , const  M2_arrayint  & m2array ) ;
     

    static UTT   M2arrayGetDegree( const  M2_arrayint &  m2array )  ;

  public:
    // ring informational
   UTT   characteristic() const { return mCharac; }

    /** @name IO
    @{ */
            void text_out(buffer &o) const { o << "GF(" << mCharac << "," << mDimension << ",Givaro)"; }

            void elem_text_out(buffer &o, 
                                const  ElementType a,
                                bool p_one, 
                                bool p_plus, 
                                bool p_parens) const;

    /** @} */


    /** @name properties
    @{ */
        bool is_unit(const ElementType f) const ;
        bool is_zero(const ElementType f) const ;
    /** @} */


        /** @name translation functions
        @{ */
    void to_ring_elem(ring_elem &result, const ElementType &a) const
    {
      result.int_val = a;
    }
 
    void from_ring_elem(ElementType &result, const ring_elem &a) const
    {
      result = a.int_val;
    }
    /** @} */

  /** @name operators
    @{ */
        bool is_equal(const ElementType f,const ElementType g) const    ;
        int compare_elems(const ElementType f,const ElementType g) const ;
    /** @} */

    /** @name get functions
        @{ */
        int get_int(elem f) const ;
        int get_repr(elem f) const ;
        M2_arrayint getModPolynomialCoeffs() const;
        M2_arrayint getGeneratorCoeffs() const;

        ring_elem   getGenerator() const;
      
    /** @} */


    /** @name init_set
    @{ */

        void init_set(elem &result, elem a) const { result = a; }

        void set(elem &result, elem a) const { result = a; }

        void init(elem &result) const ;

        void clear(elem &result) const ;

        void set_zero(elem &result) const ;

        void copy(elem &result,const elem a) const ;

        void set_from_int(elem &result, int a) const ;

        void set_from_mpz(elem &result,const mpz_ptr a) const ;

        void set_from_mpq(elem &result,const mpq_ptr a) const ;

        bool set_from_BigReal(elem &result, gmp_RR a) const { return false; }

        void set_var(elem &result, int v) const         { result = 1; }

    /** @} */


    /** @name arithmetic
    @{ */
        void negate(elem &result,const elem a) const;

        void invert(elem &result,const elem a) const;

        void add(elem &result, const elem a,const elem b) const;

        void subtract(ElementType &result,const  ElementType a,const  ElementType b) const;

        void subtract_multiple(elem &result,const  elem a,const  elem b) const;

        void mult(elem &result,const  elem a,const  elem b) const;

         ///@brief test doc
        void divide(elem &result,const  elem a,const  elem b) const;

        void power(elem &result,const  elem a,const  STT n) const;

        void power_mpz(elem &result,const  elem a,const  mpz_ptr n) const;

        void syzygy(const ElementType a, const ElementType b,
                    ElementType &x, ElementType &y) const;
    /** @} */


    /** @name misc
    @{ */
            void swap(ElementType &a, ElementType &b) const;


            void random(FieldType::randIter &it, ElementType &result) const;
            void random(ElementType &result) const;
            
    /** @} */

    bool promote(const Ring *Rf, const ring_elem f, ElementType &result) const;

    bool lift(const Ring *Rg, const ElementType f, ring_elem &result) const;

    // map : this --> target(map)
    //       primelem --> map->elem(first_var)
    // evaluate map(f)
    void eval(const RingMap *map, const elem f, int first_var, ring_elem &result) const;

  };

};


#endif
#endif

// Local Variables:
// compile-command: "make -C $M2BUILDDIR/Macaulay2/e  "
// indent-tabs-mode: nil
// End:
