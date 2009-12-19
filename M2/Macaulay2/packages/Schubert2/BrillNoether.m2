Curve = g -> (
     B := QQ[P]/P^2;
     C := abstractVariety(1,B);
     integral C := coefficient_P;
     C.TangentBundle = abstractSheaf(C, Rank => 1, ChernClass => 1 + (2-2*g)*P);
     C)

Picard = g -> (
     B := QQ[theta,Join=>false]/theta^(g+1);
     pic := abstractVariety(g,B);
     pic.TangentBundle = OO_pic^g;
     integral B := f -> (
	  i := g! * coefficient(theta^g,f);
	  try lift(i,ZZ) else lift(i,QQ));
     pic)

CxPic = (g,d) -> (
     B := QQ[theta, gamma, eta]/(
	  gamma^3,gamma*eta,
	  eta^2,
	  gamma^2+2*eta*theta,
	  theta^g*gamma,
	  theta^(g+1)
	  );
     use B;
     C := Curve g;
     Pic := Picard g;
     CxPic := abstractVariety(g+1, B);
     integral B := f -> (
	  i := g! * coefficient(theta^g * eta,f);
	  try lift(i,ZZ) else lift(i,QQ));
     CxPic.TangentBundle = abstractSheaf(CxPic,
	  Rank => dim CxPic, 
	  ChernClass => 1 + (2-2*g) * eta);
     CxPic.PoincareBundle = abstractSheaf(CxPic,
	  Rank => 1,
	  ChernClass => 1 + d*eta + gamma);
     pf2 = method();
     pi1 := new AbstractVarietyMap from {
	  symbol source => CxPic,
	  symbol target => C
	  -- PullBack => 
	  -- PushForward => 
	  -- SectionClass => 
	  };
     pi2 := new AbstractVarietyMap from {
	  symbol source => CxPic,
	  symbol target => Pic,
	  TangentBundle => abstractSheaf(CxPic, Rank => 1, ChernClass => 1 + (2-2*g) * eta),
	  PushForward => pf2
	  -- PullBack => 
	  -- SectionClass => 
	  };
     D := intersectionRing Pic;
     rm := map(D, B, {D_symbol theta, 0,0});
     pf2 B := b -> (
	  rm first first entries last coefficients(b, Variables => {eta}, Monomials => {eta})
	  );
     pf2 AbstractSheaf := E -> abstractSheaf(Pic,
	  ChernCharacter => pf2 (ch E * todd pi2)
	  );
     CxPic.projections = {pi1,pi2};
     CxPic);

BrillNoetherBundle = (g,r,d) -> (
     pic := Picard g;
     e := g+1;						 -- minimize this later
     rankA = d+1-g+e;
     kernelBundle(rankA -(r+1), OO_pic^e, 
	  abstractSheaf(pic, Rank => rankA,
	       ChernClass => sum for i from 0 to g list (-1)^i/i! * theta^i
	       )))

end

load "BrillNoether.m2"
E = BrillNoetherBundle(5,1,4)
Wrd = variety E
euler Wrd
assert(euler Wrd === -20)

load "BrillNoether.m2"
CxP = CxPic(3,5)
assert( ch CxP.PoincareBundle == 1 + (gamma + 5*eta) - theta*eta )
pi2 = CxP.projections#1
E = pi2_* CxP.PoincareBundle
ch E
use ring oo
assert( ch E == 3 - theta )