function [L0,phi0] = get_legparam(phi1,phi4)
    l5=0.15;
    l1=0.15;
    l2=0.27;
    l3=0.27;
    l4=0.15;
    YD = l4*sin(phi4);
    YB = l1*sin(phi1);
    XD = l5 + l4*cos(phi4);
    XB = l1*cos(phi1); 
    lBD = sqrt((XD - XB)*(XD - XB) + (YD - YB)*(YD - YB));
    A0 = 2*l2*(XD - XB);
    B0 = 2*l2*(YD - YB);
    C0 = l2*l2 + lBD*lBD - l3*l3;
    phi2 = 2*atan2((B0 + sqrt(A0*A0 + B0*B0 - C0*C0)),(A0 + C0));
    XC = l1*cos(phi1) + l2*cos(phi2);
    YC = l1*sin(phi1) + l2*sin(phi2);
    L0 = sqrt((XC - l5/2)*(XC - l5/2) + YC*YC);
    phi0=atan2(YC,(XC-l5/2));
end


