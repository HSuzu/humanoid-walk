function [theta] = joint_function(t, Ap, An, neutralAngle, phase, T)
    sp = t/T + phase/(2*pi);
    A = Ap*ones(size(t,1), size(t,2));
    if(sp >= 0)
        A(sp - floor(sp) > 0.5) = An;
    else
        sp = -sp;
        A(sp - floor(sp) < 0.5) = An;
    end
    
    theta = neutralAngle + A.*sin(2*pi*t/T + phase);
end

