clear all
close all
clc
global N 
obj=VideoWriter('c_g_bessel_ep_2p3_n_17.avi');
figh=figure('units','pixels','position',[0 0 1280 720]);
obj.Quality=100;
obj.FrameRate=15;
open(obj);
cm = @colors;
j1=xlsread('besselj1.xlsx');
N=34;
Rd=4.282;kn=j1(1:N);
rhat=linspace(0,Rd,500);
eps=1.0;k1=16.4706/Rd;
a0=eps/k1;epsn=a0/Rd;
b_d=0.8;b=b_d/Rd;
r=rhat/Rd;
t_hat=0:0.001:0.488;
gg=981;
t0n=t_hat*sqrt(gg/Rd);
N=length(kn);
x0=zeros(2*N,1);
for n=1:N
fun=@(x)-epsn*x.*(1-(x/b).^2).*exp(-(x/b).^2).*besselj(0,kn(n)*x);
den=@(x) x.*besselj(0,kn(n)*x).*besselj(0,kn(n)*x);
x0(n)=integral(fun,0,1)/integral(den,0,1);
end
[tn_lin,xn_lin]=ode45(@coeff_nayfeah_lin,t0n,x0);
[tn,xn]=ode45(@coeff_nayfeah,t0n,x0);

dt=0.001*sqrt(gg/Rd);
for ii=1:length(t0n)

 % nonlinear
    sum=0;sum_l=0;
    for ll=1:N
        sum=sum+besselj(0,kn(ll)*r)*xn(ii,ll);
        sum_l=sum_l+besselj(0,kn(ll)*r)*xn_lin(ii,ll);
    end
    etan=sum;etal=sum_l;
    plot(r*Rd,etan*Rd,'r','LineWidth',4)
    hold on
    plot(-r*Rd,etan*Rd,'r','LineWidth',4)
     plot(r*Rd,etal*Rd,'g--','LineWidth',4)
    hold on
    plot(-r*Rd,etal*Rd,'g--','LineWidth',4)
    % simulation 
    str = sprintf('wave-%g.dat',t_hat(ii)/0.001);
    data = importdata(str);
    jj = length(data);
    X=data(:,1);Y=data(:,2);
    plot(Y(1:5:end),X(1:5:end),'b*','MarkerSize',4)
    plot(-Y(1:5:end),X(1:5:end),'b*','MarkerSize',4)
    hold off
    axis([-2 2 -1.1*a0 1.2*a0])
    drawnow
    f=getframe(figh);
    writeVideo(obj,f);
    y_ini=[];eta_lin=[];
end
obj.close();