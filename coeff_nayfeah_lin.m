function dxdt=coeff_nayfeah_lin(tn,xn)
global N r
j1=xlsread('besselj1.xlsx');
ss=0.004;
Rd=4.282;kn=j1(1:N);
omn=sqrt(kn.*(1+ss*kn.^2));
dump=zeros(2*N,1);
%xx=linspace(-1,1,50);
for n=1:N


    dump(n)=xn(n+N,1);
    dump(n+N)=-omn(n)^2*xn(n,1);
end

dxdt=dump;
