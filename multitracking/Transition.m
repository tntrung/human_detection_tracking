function [T,pospartical]=Transition(T)
Parameter;                              %private parameter
pospartical=struct('data',zeros(0,2));  %save position particles (for paint particles)
for tr=1:length(T)        
    %test particle 1 (max weight)
    update_partical_1=0;
    if DistantPoint([T(tr).p(1).x T(tr).p(1).y],[T(tr).cx T(tr).cy])>(T(tr).height+T(tr).width)/rate_distanttrackpartical || T(tr).p(1).w<thresholdweight
        vec=normrnd(0,1/4,[1 2]);           %error rate vector
        %estimate vector
        T(tr).p(1).u=vec(1);                %set vector with error rate for particle
        T(tr).p(1).v=vec(2);                %
        T(tr).p(1).w=0; 
        posi=normrnd(0,1/4,[1 2]);          %error rate position
        %estimate position
        T(tr).p(1).x=T(tr).cx+T(tr).u*numframe_persecond+((T(tr).height+T(tr).width)/rate_distanttrackpartical)*posi(1);%T(tr).cx+((T(tr).width))*posi(1)+vec(1); %-denta_t de no nam trong detector
        T(tr).p(1).y=T(tr).cy+T(tr).v*numframe_persecond+distant_width_height*((T(tr).height+T(tr).width)/rate_distanttrackpartical)*posi(2);%T(tr).cy+((T(tr).height/2))*posi(2)+vec(2);                   
        %save position particles (for paint particles)
        pospartical(tr).data(1,:)=[T(tr).p(1).x T(tr).p(1).y];
        update_partical_1=1;
    else        
        T(tr).p(1).x=T(tr).p(1).x+T(tr).positionupdate(1);
        T(tr).p(1).y=T(tr).p(1).y+T(tr).positionupdate(2);
    end    
    meanw=T(tr).p(1).w*minrate_particlemax;
    %test for other particles
    for p=2:num_particles
        if DistantPoint([T(tr).p(p).x T(tr).p(p).y],[T(tr).cx T(tr).cy])>(T(tr).height+T(tr).width)/rate_distanttrackpartical || T(tr).p(p).w<meanw
            if update_partical_1==0                 %particle 1 good
                %get vector from partical 1
                T(tr).p(p).u=T(tr).p(1).u;          %
                T(tr).p(p).v=T(tr).p(1).v;          %
            else                                    %init new particle
                %estimate vector
                vec=normrnd(0,1/4,[1 2]);           %error rate vector
                T(tr).p(p).u=vec(1);                %
                T(tr).p(p).v=vec(2);                %
            end
            %estimate position
            posi=normrnd(0,1/4,[1 2]);
            T(tr).p(p).x=T(tr).cx+T(tr).p(p).u*numframe_persecond+((T(tr).height+T(tr).width)/rate_distanttrackpartical)*posi(1);
            T(tr).p(p).y=T(tr).cy+T(tr).p(p).v*numframe_persecond+distant_width_height*((T(tr).height+T(tr).width)/rate_distanttrackpartical)*posi(2);
        else        
            T(tr).p(p).x=T(tr).p(p).x+T(tr).positionupdate(1);
            T(tr).p(p).y=T(tr).p(p).y+T(tr).positionupdate(2);
        end        
        %save position particles (for paint particles)
        pospartical(tr).data(p,:)=[T(tr).p(p).x T(tr).p(p).y];
    end    
end