syms a b;

h = 1-a-b;
M=sym(zeros(9));
M(1,1)=h;
M(1,2)=h;
M(1,3)=h;
M(1,4)=h;
M(1,5)=h;
M(1,6)=h;
M(1,7)=b*(1-b)+h;
M(1,8)=a*(1-a)+h;
M(1,9)=1;

M(2,1)=b;

M(3,1)=a;

M(4,2)=b;
M(4,4)=b;
M(4,7)=b*b;


M(5,2)=a;
M(5,3)=b;
M(6,3)=a;
M(6,6)=a;
M(6,8)=a*a;

M(7,4)=a;
M(7,5)=b;

M(8,5)=a;
M(8,6)=b;

M(9,7)=a;
M(9,8)=b;

I=eye(9);

A=M-I;
[V,D]=eig(M);
C=V(:,2);


total = 0;
for i=1:9
total=total+D(i);    
end


rpool1 = C(3)*h+2*C(6)*h+h*C(8)*2+a*C(5)*h+.25*C(5)*h;
rpool2 = C(2);
rhonest = C(1)*h+C(5)*.25+h*.25*C(5)+C(5)*h*h;
D2=C(1)+C(2)+C(3)+C(4)+C(5)+C(6)+C(7)+C(8)+C(9);

N = C/D2;

p2 = b;
p3 = a;
p4 = (b^2+2*b^4*a)/(1-b-b^2*a);
p5 = 2*a*b;
p6 = (a^2+2*a^4*b)/(1-a-a^2*b);
p7 = (a*b^2+2*b^4*a^2)/(1-b-b^2*a)+2*a*b^2;
p8 = 2*a^2*b+(b^2*a+2*a^4*b^2)/(1-a-a^2*b);
p9 = a*p7+b*p8;
G=1+p2+p3+p4+p5+p6+p7+p8+p9;


//Rp = h*N(3)+2*N(6)*a+h*N(6)*2+h*N(8)*2+h*N(9)*(.5*2+3*a)+N(5)*h*(.24*h+2*a);
gamma_12 = 0.5;
gamma_12H = 0.333;
//The reward function for player 1 (selfish miner 1) is as follows:
Rp = (N(3)*a*(h/2)) + (2*N(3)*a) + (N(3)*a*b) + (N(6)*a) + (2*N(6)*a) + (2*N(8)*h) + (2*N(8)*a^2) + (3*N(8)*(a*(1-a))) + (2*N(9)*gamma_12*h) + (3*N(9)*a) + (3*N(8)*h*a) + (N(5)*gamma_12H) + (2*N(5)*a);

//Rh = N(1)*h+N(5)*.25+h*.25*N(5)+N(5)*h*h;

//The reward for the honest miner is as follows:
Rh = (N(1)*h) + (2*N(3)*(h/2)) + (2*N(3)*(h/2)) + (N(9)*gamma_12*h) + (N(9)*gamma_12*h) + (N(5)*gamma_12H) + (N(5)*gamma_12H);

//The reward function for player 2 (selfish miner 2) is as follows:
Rp2 = (N(3)*b*(h/2)) + (2*N(3)*b) + (N(3)*b*a) + (N(6)*b) + (2*N(6)*b) + (2*N(8)*h) + (2*N(8)*b^2) + (3*N(8)*(b*(1-b))) + (2*N(9)*gamma_12*h) + (3*N(9)*b) + (3*N(8)*h*b) + (N(5)*gamma_12H) + (2*N(5)*b);



Re=Rp/(Rp+Rh+Rp2);

