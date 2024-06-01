function dxdt=coeff_nayfeah(tn,xn)
global N r
j1=xlsread('besselj1.xlsx');
ss=0.004;
Rd=4.282;kn=j1(1:N);
omn=sqrt(kn.*(1+ss*kn.^2));
dump=zeros(2*N,1);
%xx=linspace(-1,1,50);
for n=1:N
    sum=0;
    Ga=csvread(sprintf('./newdir/Ga_%d.csv',n));
    Ch=csvread(sprintf('./newdir/Ch_%d.csv',n));
    for m=1:N
        for s=1:N
            sum=sum-Ga(m,s)*xn(m)*xn(s)*omn(s)^2+Ch(m,s)*xn(N+m)*xn(N+s);
            
        end
        
    end



% term1 = (xn(1:N,1))'*(-Ga)*((omn.*2).*xn(1:N,1));
% 
% 
% term2 = (xn(N+1:end,1))'*Ch*xn(N+1:end,1);
% 
% 
% result = term1 + term2;

    dump(n)=xn(n+N,1);
    dump(n+N)=-omn(n)^2*xn(n,1)-sum;
end
fprintf("Count-%f,      \n",tn)
dxdt=dump;
