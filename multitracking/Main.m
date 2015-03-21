clear all;clc;close();fig=figure(); %clear old data and figure
Parameter;                          %private parameter for functions
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%dataset
linkdataset='..\..\';                            %dataset (input) 
namefile='image (';nameadd=').';tail='jpg';             %name file(input)
inputhog=strcat(linkdataset,'hog\ground_hog_key');%output hog (intput)
linkoutput=strcat(linkdataset,'trackhog_hm8\');       %save image (output)
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%Parameter of main
%update tracker
trackvisible=-30;       %exceed the number of hidden
abortmargin=[10 200 10 10];%margin for top,right,botom, left
usemin=-10;             %delete tracker to margin abort if use < usemin
%%%defaulse
%eval
clearmot=1;             %assessment clearmot = 1 else = 0
%paint
NL=100;                 %paint point ellipse, bezier
writeimage=1;           %write image to disk = 1 else = 0
paint_particle=1;       %paint particle to figure = 1 else = 0
paint_trajectory=1;     %paint trajectory to figure = 1 else = 0
paint_ellipse=1;        %paint ellipse to figure = 1 else = 0
%update tracker
initframe=1;            %begin frame for test
%boosting
typeft=0;               %use feature [3:haar + histogram (adaboost)] [2: histogram] [1: histogram (adaboost)], [0: texture haar (adaboost)]
minconf=-100;           %if rect no exists inside searchregion
maxa_c=20;              %max value for standard resource boosting
maxa_p=20;              %max value for standard resource boosting
maxa_h=1;               %max value for standard resource boosting
buildmex=0;             %build mexfile = 1 else = 0
searchregion=4;         %region search for boosting
baseclass=100;          %numbaseclass for boosting
numtraininit=50;        %num train for taget init boosting    
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%init processing
%test exits database and count file
numfile = dir(fullfile(linkdataset,strcat('*.',tail)));
nFrames = length(numfile);clear numfile;            
if nFrames<initframe                                      
    msgbox('loi duong dan hoac dinh dang khong phu hop');
    clear all;return;                               
end   

%read init image get height and width
frame=imread(strcat(linkdataset,namefile,int2str(initframe),nameadd,tail));
[h,w]=size(frame(:,:,1));clear frame;

%get output hog (input framework)
regions=GetDetector(inputhog,h,w,nFrames);               

%init particle for init frame
region_frame=FixDetector(regions(initframe).rec);   %get init detector
renumtrack=length(region_frame(:,1));               %save tracker number
[T,ID] = InitTracker( region_frame,renumtrack,0);   %ID frame init by 0
fittertrack=zeros(renumtrack,1);                    %array update for boosting
if buildmex==1                                      %build mexfile (boosting)
    mex mf_boostingclassifier.cpp ...    
    -I'C:\OpenCV2.3\opencv\modules\objdetect\src' ...
    -I'C:\OpenCV2.3\build\include' ...
    -I'C:\OpenCV2.3\build\include\opencv' ...    
    'C:\OpenCV2.3\build\x64\vc10\lib\opencv_core230d.lib' ...
    'C:\OpenCV2.3\build\x64\vc10\lib\opencv_highgui230d.lib' ...
    'C:\OpenCV2.3\build\x64\vc10\lib\opencv_imgproc230d.lib' ...
    'C:\OpenCV2.3\build\x64\vc10\lib\opencv_objdetect230d.lib'
end
mf_boostingclassifier(0,typeft,strcat(linkdataset,namefile,int2str(initframe),nameadd,tail),...
    renumtrack,searchregion,region_frame',renumtrack,fittertrack,baseclass,numtraininit);

%write value for assessment clear mot
if clearmot==1                                      
    groundtruth = fopen(strcat(linkoutput,'ground_track_key'),'w'); 
    for gt=1:renumtrack                             %create file and write intit tracker
        stringname=strcat('0_',int2str(gt-1),'_',int2str(T(gt).cx),'_',int2str(T(gt).cy),'_',int2str(T(gt).ID),'_0_',int2str(T(gt).x),'_',int2str(T(gt).y),'_',int2str(T(gt).width),'_',int2str(T(gt).height),'\r\n');%,'_0_0_0_0_0_0_0_0_0_0_0_0_0\r\n');
        stringname(ismember(stringname,' ,.:;!_')) = ' ';
        fprintf(groundtruth,stringname);                
    end        
end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%processing
disp('frame 1 Init tracker');t = clock;                     %sum time for frame work
for i=initframe+1:nFrames    
    if i==950                                               %debug in frame
        ddi=580;
    end
    disp(strcat('frame ',int2str(i)));    
    linkimage=strcat(linkdataset,namefile,int2str(i),nameadd,tail);    
    region_frame=FixDetector(regions(i).rec);               %get detector index i
    numdetect=length(region_frame(:,1));                    %num detector frame
    
    rectparticle=zeros(renumtrack*num_particles,4); %boosting for partical
    for trp=1:renumtrack
        for p=1:num_particles
            rectparticle(p+num_particles*(trp-1),:)=[T(trp).p(p).x T(trp).p(p).y 0 0];                
        end 
    end         
    if renumtrack~=0
    %eval output: tracker for detect, partical, density hog
        class_all=mf_boostingclassifier(1,typeft,linkimage,renumtrack,searchregion,region_frame',numdetect,rectparticle',num_particles,minconf)';
        %class_all=StandardMatrix_Row(class_all);
    else
        class_all=zeros(0,num_particles*2)+numdetect;
    end
    %get classifier score detector
    %[c_tr_d_score,maxa]=StandardMatrix([-5 -100],maxa);
    [c_tr_d_score,maxa_c]=StandardMatrix(class_all(1:end,1:numdetect),maxa_c);
    %c_tr_d_score=class_all(1:end,1:numdetect);
    %get classifier score partical
    [c_p_tr_class,maxa_p]=StandardMatrix(class_all(1:end,numdetect+1:num_particles+numdetect),maxa_p);              
    %c_p_tr_class=class_all(1:end,numdetect+1:num_particles+numdetect);              
    %get score density hog
    [hog_p_score,maxa_h]=StandardMatrix(class_all(1:end,numdetect+num_particles+1:end),maxa_h);      
    %hog_p_score=class_all(1:end,numdetect+num_particles+1:end);
    if ~isempty(T) || ~isempty(region_frame)
        %update weight for partical
        [T,hypo,ID]=UpdateWeight(T,region_frame,c_tr_d_score,c_p_tr_class,hog_p_score,ID);                
    end    
    %compute tracker update,delete,init for boosting and framework
    numrect=length(T);fittertrack=zeros(0,1);sotrackcu=0;
    for tr=numrect:-1:1                         
        %if T(tr).x<0 || T(tr).y<0|| T(tr).y+T(tr).height>h || T(tr).x+T(tr).width>w || hypo(tr,1)==-3
            %delete tracker out off whole image || num track < min track
            %T(tr).use=trackvisible-1;
        %end
        if (T(tr).use<1) && (T(tr).x<abortmargin(4) || T(tr).y<abortmargin(1) || T(tr).y+T(tr).height>h-abortmargin(3) || T(tr).x+T(tr).width>w-abortmargin(2))         
            T(tr).unuse=T(tr).unuse-1;
            %delete tracker if use < usemin and <abortmargin
            if T(tr).unuse<usemin %delete tracker
                T(tr).use=trackvisible-1;
            end            
        else
            T(tr).unuse=0;
        end        
        if T(tr).use == 2
            fittertrack(tr)=0;      %init        
        elseif T(tr).use < trackvisible %|| T(tr).positionupdate(1)+T(tr).x+T(tr).width/2>w-1 || T(tr).positionupdate(2)+T(tr).y+T(tr).height/2>h-1 || T(tr).positionupdate(1)+T(tr).x<1 || T(tr).positionupdate(2)+T(tr).y<1%|| T(tr).x+T(tr).width>w-5 || T(tr).y+T(tr).height>h-5 || T(tr).x<5 || T(tr).y<5                      
            T(tr)=[];
            numrect=numrect-1;
            fittertrack(tr)=-1;     %delete            
        elseif T(tr).use~=2                
            sotrackcu=sotrackcu+1;  
            fittertrack(tr)=1;      %update  
            if hypo(tr,1)==-2
                fittertrack(tr)=-2; %nothing            
            end       
        end            
    end               
    
    [T,pospartical]=Transition(T);  %motion model + resample
    
    if clearmot==1                  %write assessment clear mot
        for gt=1:numrect
            stringname=strcat(int2str(i-1),'_',int2str(gt-1),'_',int2str(T(gt).cx),'_',int2str(T(gt).cy),'_',int2str(T(gt).ID),'_',int2str(i-1),'_',int2str(T(gt).x),'_',int2str(T(gt).y),'_',int2str(T(gt).width),'_',int2str(T(gt).height),'\r\n');%,'_0_0_0_0_0_0_0_0_0_0_0_0_0\r\n');
            stringname(ismember(stringname,' ,.:;!_')) = ' ';
            fprintf(groundtruth,stringname);                
        end    
    end
    %update boosting    
    numtrack=length(T);
    recttrack=zeros(numtrack,4); 
    for tr=1:numtrack            
        recttrack(tr,:)=[T(tr).x T(tr).y T(tr).width T(tr).height];                                                                         
    end     
    
    if numtrack==0
        clear mf_boostingclassifier
    else
        mf_boostingclassifier(0,typeft,linkimage,numtrack,searchregion,recttrack',length(fittertrack),fittertrack,baseclass,numtraininit);
    end
    renumtrack=numrect;             %update numtracker
    clf();
    %paint to screen
    frame=imread(linkimage);        %read image
    if sotrackcu==0                 %only show image if = 0
        imshow(frame);  
        if writeimage==1
            saveas(fig,strcat(linkoutput,int2str(i),'.jpg'),'jpg');            
        end
        clear frame;
        continue;
    end        
    hold off; 
    imshow(frame);        
    hold on;   
    for itr=1:numrect       
        if T(itr).use==2            %don't show tracker init
            %continue;
        end
        %drawn particle
        if paint_particle==1        %paint partile if = 1
            for par=1:num_particles
                plot(pospartical(itr).data(par,1),pospartical(itr).data(par,2),'.');
            end   
        end
        %drawn elip       
        if paint_ellipse==1        
            rectangle('Position',[T(itr).x T(itr).y T(itr).width T(itr).height],'Curvature',[1 1],'EdgeColor',T(itr).color(1:3),'LineWidth',2)        
        end
        %drawn bezier
        if paint_trajectory==1
            if length(T(itr).tr)>1
                point=zeros(2,0);poin=1;
                for ip=length(T(itr).tr):-1:1
                    point(:,poin)=[T(itr).tr(ip).y T(itr).tr(ip).x];poin=poin+1;
                end               
                X = Bezier(point,NL);
                line(X(2,:),X(1,:),'LineStyle','-','LineWidth',2,'Color',T(itr).color(1:3));            
            end          
        end
        clear X point;
    end     
    if writeimage==1
        saveas(fig,strcat(linkoutput,int2str(i),'.jpg'),'jpg');
    end
    clear frame;
end
disp('Sum time for framework:');
disp(etime(clock, t));
clear all;