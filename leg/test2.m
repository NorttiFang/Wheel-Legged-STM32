F0 = 55;
Tp = 50;
phi1 = 0;
phi4 = pi;

%[Tp1,Tp2,L0,phi0] = VM_FIVE(F0,Tp,phi1,phi4);
[Tp1,Tp2] = VM_cal(F0,Tp,phi1,phi4);
[L0,phi0] = get_legparam(phi1,phi4);