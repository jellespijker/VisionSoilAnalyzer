function varargout = SoilAnalyzer(varargin)
% SOILANALYZER MATLAB code for SoilAnalyzer.fig
%      SOILANALYZER, by itself, creates a new SOILANALYZER or raises the existing
%      singleton*.
%
%      H = SOILANALYZER returns the handle to a new SOILANALYZER or the handle to
%      the existing singleton*.
%
%      SOILANALYZER('CALLBACK',hObject,eventData,handles,...) calls the local
%      function named CALLBACK in SOILANALYZER.M with the given input arguments.
%
%      SOILANALYZER('Property','Value',...) creates a new SOILANALYZER or raises the
%      existing singleton*.  Starting from the left, property value pairs are
%      applied to the GUI before SoilAnalyzer_OpeningFcn gets called.  An
%      unrecognized property name or invalid value makes property application
%      stop.  All inputs are passed to SoilAnalyzer_OpeningFcn via varargin.
%
%      *See GUI Options on GUIDE's Tools menu.  Choose "GUI allows only one
%      instance to run (singleton)".
%
% See also: GUIDE, GUIDATA, GUIHANDLES

% Edit the above text to modify the response to help SoilAnalyzer

% Last Modified by GUIDE v2.5 15-Oct-2014 21:36:26

% Begin initialization code - DO NOT EDIT
gui_Singleton = 1;
gui_State = struct('gui_Name',       mfilename, ...
                   'gui_Singleton',  gui_Singleton, ...
                   'gui_OpeningFcn', @SoilAnalyzer_OpeningFcn, ...
                   'gui_OutputFcn',  @SoilAnalyzer_OutputFcn, ...
                   'gui_LayoutFcn',  [] , ...
                   'gui_Callback',   []);
if nargin && ischar(varargin{1})
    gui_State.gui_Callback = str2func(varargin{1});
end

if nargout
    [varargout{1:nargout}] = gui_mainfcn(gui_State, varargin{:});
else
    gui_mainfcn(gui_State, varargin{:});
end

% End initialization code - DO NOT EDIT

% --- Executes just before SoilAnalyzer is made visible.
function SoilAnalyzer_OpeningFcn(hObject, eventdata, handles, varargin)
% This function has no output args, see OutputFcn.
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% varargin   command line arguments to SoilAnalyzer (see VARARGIN)

% Choose default command line output for SoilAnalyzer
handles.output = hObject;

% Init Beaglebone connection
global bbb;
%bbb = beaglebone_black('192.168.178.20', 'root', 'open');
bbb = beaglebone_black('192.168.7.2', 'root', 'open');
system(bbb, './pwm_vision_matlab');

% Init the CAM
border = 0;
global cam src;
cam = videoinput('winvideo', 2, 'MJPG_2048x1536');
cam.FramesPerTrigger = 1;
hImage = image(zeros(1536, 2048, 3), 'Parent', handles.axes_Image);
preview(cam, hImage);
cam.ROIPosition = [ border border 2048-border 1536-border ];
%cam.PowerLineFrequency = 50;



% Update handles structure
guidata(hObject, handles);

% UIWAIT makes SoilAnalyzer wait for user response (see UIRESUME)
% uiwait(handles.figure1);

% --- Outputs from this function are returned to the command line.
function varargout = SoilAnalyzer_OutputFcn(hObject, eventdata, handles) 
% varargout  cell array for returning output args (see VARARGOUT);
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Get default command line output from handles structure
varargout{1} = handles.output;

% --- Executes on button press in pushbuttonSnap.
function pushbuttonSnap_Callback(hObject, eventdata, handles)
% hObject    handle to pushbuttonSnap (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
global bbb cam I L Dis labI;
system(bbb, 'echo 1 > /sys/class/gpio/gpio51/value && echo 1 > /sys/class/gpio/gpio50/value && echo 1 > /sys/class/gpio/gpio60/value');
start(cam)
I = getdata(cam);
system(bbb, 'echo 0 > /sys/class/gpio/gpio51/value && echo 0 > /sys/class/gpio/gpio50/value && echo 0 > /sys/class/gpio/gpio60/value');
L = GetLabeledImage(I);
[Dis labI] = CIELabSoilColorDistribution(L, I);

axes(handles.axes_Image);
aImage = I;
image(aImage);

% Update handles structure
guidata(hObject, handles);

% --- Executes on button press in pushbuttonAS.
function pushbuttonAS_Callback(hObject, eventdata, handles)
% hObject    handle to pushbuttonAS (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

global L dis I P;
% Get CIElab scatterplot
L = GetLabeledImage(I);
dis = CIELabSoilColorDistribution(L,I);
axes(handles.axes_CIE);
ma = mean(dis(:,1));
mb = mean(dis(:,2));
handels.axes_CIE = scatter(dis(:,1),dis(:,2),'.');
hold on
plot([ma, ma],ylim,'r--','LineWidth',1);
plot(xlim, [mb, mb],'r--','LineWidth',1);
hold off

% Get RI Histogram
RI = RISoilColorDistribution(L, I);
axes(handles.axes_RI);
mri = mean(RI);
handels.axes_RI = hist(RI);
hold on
plot([mri, mri],ylim,'r--','LineWidth',1);
hold off

% Get PSD CDF plot
P = PSD(L);
axes(handles.axes_PSD);
handels.axes_PSD = cdfplot(P);

% Get the textuur histogram


% --- Executes on button press in pushbutton3.
function pushbutton3_Callback(hObject, eventdata, handles)
% hObject    handle to pushbutton3 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
