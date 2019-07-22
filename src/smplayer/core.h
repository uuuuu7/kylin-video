/*  smplayer, GUI front-end for mplayer.
    Copyright (C) 2006-2015 Ricardo Villalba <rvm@users.sourceforge.net>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef _CORE_H_
#define _CORE_H_

#include <QObject>
#include <QProcess> // For QProcess::ProcessError
#include "mediadata.h"
#include "mediasettings.h"
#include "playerprocess.h"

#include "config.h"

class FileSettingsBase;

//class FileSettings;
class PlayerProcess;
class MplayerWindow;
class QSettings;

class Core : public QObject
{
    Q_OBJECT
    
public:
    enum State { Stopped = 0, Playing = 1, Paused = 2, Buffering = 3 };

    Core(MplayerWindow *mpw, const QString &snap = QString::null, QWidget* parent = 0);
	~Core();

	MediaData mdat;
	MediaSettings mset;

	//! Return the current state
	State state() { return _state; };

	//! Return a string with the name of the current state,
	//! so it can be printed on debugging messages.
	QString stateToString();

	void addForcedTitle(const QString & file, const QString & title) { forced_titles[file] = title; };

protected:
	//! Change the current state (Stopped, Playing or Paused)
	//! And sends the stateChanged() signal.
	void setState(State s);
    
public slots:
	//! Generic open, with autodetection of type
	void open(QString file, int seek=-1); 
	void openFile(QString filename, int seek=-1);
    void openStream(QString name, QStringList params = QStringList());
	/*
	void openDVD( bool from_folder, QString directory = "");
	void openDVD(); // Plays title 1
	void openDVD(int title = 1);
	*/
	void openDVD(QString dvd_url);
//#ifdef BLURAY_SUPPORT
//	void openBluRay(QString blu_ray_url);
//#endif
	void openVCD(int title = -1);
	void openAudioCD(int title = -1);
//	void openTV(QString channel_id);


	void loadSub(const QString & sub);
	void unloadSub();

	//! Forces to use the specified subtitle file. It's not loaded immediately but stored
	//! and will be used for the next video. After that the variable is cleared.  
	void setInitialSubtitle(const QString & subtitle_file) { initial_subtitle = subtitle_file; };

	void loadAudioFile(const QString & audiofile);
	void unloadAudioFile();

    void stop(); 
    void play();
	void play_or_pause();
    void pause_and_frame_step();
	void pause();
	void frameStep();
	void frameBackStep();

    void screenshot();      //!< Take a screenshot of current frame
     void screenshots();     //!< Start/stop taking screenshot of each frame

     void screenshot(bool include_subtitles);
     void screenshotWithSubtitles();
     void screenshotWithoutSubtitles();
//#ifdef CAPTURE_STREAM
//	void switchCapturing();
//#endif

	//! Public restart, for the GUI.
	void restart();

	//! Reopens the file (no restart)
	void reload();

	void goToPosition( int value );
	void goToPos( double perc );

	void goToSec( double sec );
	void goToSec(int sec) { goToSec( (double) sec); }

	void setAMarker(); //!< Set A marker to current sec
	void setAMarker(int sec);

	void setBMarker(); //!< Set B marker to current sec
	void setBMarker(int sec);

	void clearABMarkers();

    void toggleRepeat();
    void toggleRepeat(bool b);

	void toggleFlip();
	void toggleFlip(bool b);

	void toggleMirror();
	void toggleMirror(bool b);

	// Audio filters
//#ifdef MPLAYER_SUPPORT
    void toggleKaraoke();
    void toggleKaraoke(bool b);
    void toggleExtrastereo();
    void toggleExtrastereo(bool b);
//#endif

    void toggleVolnorm();
    void toggleVolnorm(bool b);

//#ifdef MPV_SUPPORT
    void toggleEarwax();
    void toggleEarwax(bool b);
//#endif

	void setAudioChannels(int channels);
	void setStereoMode(int mode);

	// Video filters
	void toggleAutophase();
	void toggleAutophase(bool b);
	void toggleDeblock();
	void toggleDeblock(bool b);
	void toggleDering();
	void toggleDering(bool b);
	void toggleGradfun();
	void toggleGradfun(bool b);
	void toggleNoise();
	void toggleNoise(bool b);
	void togglePostprocessing();
	void togglePostprocessing(bool b);
	void changeDenoise(int);
	void changeUnsharp(int);
	void changeLetterbox(bool);
	void changeLetterboxOnFullscreen(bool);
	void changeUpscale(bool);
	void changeStereo3d(const QString & in, const QString & out);

	void seek(int secs);
	void sforward(); 	// + 10 seconds
	void srewind(); 	// - 10 seconds
	void forward(); 	// + 1 minute
	void rewind(); 		// -1 minute
	void fastforward();	// + 10 minutes
	void fastrewind();	// - 10 minutes
	void forward(int secs);
	void rewind(int secs);
//#ifdef MPV_SUPPORT
    void seekToNextSub();
    void seekToPrevSub();
//#endif
	void wheelUp();
	void wheelDown();

	void setSpeed( double value );
	void incSpeed10();	//!< Inc speed 10%
	void decSpeed10();	//!< Dec speed 10%
	void incSpeed4();	//!< Inc speed 4%
	void decSpeed4();	//!< Dec speed 4%
	void incSpeed1();	//!< Inc speed 1%
	void decSpeed1();	//!< Dec speed 1%
	void doubleSpeed();
	void halveSpeed();
	void normalSpeed();

	void setVolume(int volume, bool force = false);
	void switchMute();
	void mute(bool b);
    void incVolume(int step);
    void decVolume(int step);
    void incVolume();
    void decVolume();

    bool getMute();//kobe
    int getVolumn();//kobe

	void setBrightness(int value);
	void setContrast(int value);
	void setGamma(int value);
	void setHue(int value);
	void setSaturation(int value);

	void incBrightness();
	void decBrightness();
	void incContrast();
	void decContrast();
	void incGamma();
	void decGamma();
	void incHue();
	void decHue();
	void incSaturation();
	void decSaturation();

	void setSubDelay(int delay);
	void incSubDelay();
	void decSubDelay();

	void setAudioDelay(int delay);
	void incAudioDelay();
	void decAudioDelay();

	void incSubPos();
	void decSubPos();

    void changeSubScale(double value);
    void incSubScale();
    void decSubScale();

    void changeOSDScale(double value);
    void incOSDScale();
    void decOSDScale();

    void setOSDFractions(bool active);

	//! Select next line in subtitle file
	void incSubStep();
	//! Select previous line in subtitle file
	void decSubStep();

	void changeSubVisibility(bool visible);

	void changeExternalSubFPS(int fps_id);

	//! Audio equalizer
    void setAudioEqualizer(AudioEqualizerList values, bool restart = false);
    void setAudioAudioEqualizerRestart(AudioEqualizerList values) { setAudioEqualizer(values, true); };
	void updateAudioEqualizer();

	void setAudioEq(int eq, int value);
	void setAudioEq0(int value);
	void setAudioEq1(int value);
	void setAudioEq2(int value);
	void setAudioEq3(int value);
	void setAudioEq4(int value);
	void setAudioEq5(int value);
	void setAudioEq6(int value);
	void setAudioEq7(int value);
	void setAudioEq8(int value);
	void setAudioEq9(int value);

	void changeDeinterlace(int);
    void changeSubtitle(int track);
	void nextSubtitle();
//#ifdef MPV_SUPPORT
    void changeSecondarySubtitle(int track);
//#endif
	void changeAudio(int ID, bool allow_restart = true);
	void nextAudio();
	void changeVideo(int ID, bool allow_restart = true);
	void nextVideo();

	void changeTitle(int);
	void changeChapter(int);
	void prevChapter();
	void nextChapter();
	void changeAngle(int);
	void changeAspectRatio(int);
	void nextAspectRatio();
	void changeOSD(int);
	void nextOSD();
	void nextWheelFunction();

//#ifdef BOOKMARKS
    void nextBookmark();
    void prevBookmark();
    void saveBookmarks();
//#endif

//	#if 0
//	void changeSize(int); // Size of the window
//	void toggleDoubleSize();
//	#endif
	void changeZoom(double); // Zoom on mplayerwindow

	void changeRotate(int r);

    void changeAO(const QString & new_ao);

	void incZoom();
	void decZoom();
	void resetZoom();
	void autoZoom();
	void autoZoomFromLetterbox(double video_aspect);
	void autoZoomFor169();
	void autoZoomFor235();

	void showFilenameOnOSD();
    void showMediaInfoOnOSD();
	void showTimeOnOSD();
	void toggleDeinterlace();

    void changeUseCustomSubStyle(bool);
    void toggleForcedSubsOnly(bool);

    void changeClosedCaptionChannel(int);
	/*
	void nextClosedCaptionChannel();
	void prevClosedCaptionChannel();
	*/

//#if DVDNAV_SUPPORT
//	// dvdnav buttons
//	void dvdnavUp();
//	void dvdnavDown();
//	void dvdnavLeft();
//	void dvdnavRight();
//	void dvdnavMenu();
//	void dvdnavSelect();
//	void dvdnavPrev();
//	void dvdnavMouse();
//#endif

	//! Change fullscreen when using the player own window
	void changeFullscreenMode(bool b);

	//! Wrapper for the osd_show_text slave command
	void displayTextOnOSD(QString text, int duration = 3000, int level = 1, 
                          QString prefix = QString::null);

public:
	//! Returns the number of the first chapter in 
	//! files. In some versions of mplayer is 0, in others 1
    static int firstChapter();
	int firstDVDTitle();
	int firstBlurayTitle();

//#ifndef NO_USE_INI_FILES
	void changeFileSettingsMethod(QString method);
//#endif

protected:
	//! Returns the prefix to keep pausing on slave commands
	QString pausing_prefix();
	void seek_cmd(double secs, int mode);

protected slots:
    void changeCurrentSec(double sec);
    void changePause();
	void gotWindowResolution( int w, int h );
	void gotNoVideo();
	void gotVO(QString);
	void gotAO(QString);
	void gotStartingTime(double);
	void gotVideoBitrate(int);
	void gotAudioBitrate(int);

	void finishRestart();
    void processFinished();
	void fileReachedEnd();
    
	void displayMessage(QString text);
	void displayScreenshotName(QString filename);
	void displayUpdatingFontCache();
	void displayBuffering();
	void displayPlaying();

	void streamTitleChanged(QString);
	void streamTitleAndUrlChanged(QString,QString);

	// Catches mediaInfoChanged and sends mediaPlaying signal
	void sendMediaInfo();
	
	void watchState(Core::State state);

	//! Called when a video has just started to play.
	//! This function checks if the codec of video is ffh264 and if
	//! the resolution is HD
	void checkIfVideoIsHD();

    void initAudioTrack(const Tracks &, int selected_id);

    void initVideoTrack(const Tracks &, int selected_id);

    void initSubtitleTrack(const SubTracks &, int selected_id);
	void setSubtitleTrackAgain(const SubTracks &);
    void updateChapterInfo(const Chapters &);

//#if DVDNAV_SUPPORT
//	void dvdTitleChanged(int);
//	void durationChanged(double);
//	void askForInfo();
//	void dvdnavUpdateMousePos(QPoint);
//	void dvdTitleIsMenu();
//	void dvdTitleIsMovie();
//#endif

	void initializeOSD();


//#if defined(Q_OS_WIN) || defined(Q_OS_OS2)
//#ifdef SCREENSAVER_OFF
//	void enableScreensaver();
//	void disableScreensaver();
//#endif
//#endif

protected:
	void playNewFile(QString file, int seek=-1);
	void restartPlay();
	void initPlaying(int seek=-1);
	void newMediaPlaying();

    void startMplayer(QString file, double seek = -1 );
	void stopMplayer();

    //#ifndef NO_USE_INI_FILES
        void saveMediaInfo();
    //#endif
    void restoreSettingsForMedia(const QString & name, int type);

    void initializeMenus();
	void updateWidgets();

	//! Returns true if changing the subscale requires to restart mplayer
    bool subscale_need_restart();

	int adjustVolume(int v, int max_vol);

signals:
	void buffering();
	void aboutToStartPlaying(); // Signal emited just before to start mplayer
	void mediaLoaded();
	void mediaInfoChanged();
    void mediaDataReceived(const MediaData &);
	//! Sends the filename and title of the stream playing in this moment
	void mediaPlaying(const QString & filename, const QString & title);
	void stateChanged(Core::State state);
	void mediaStartPlay();
	void mediaFinished(); // Media has arrived to the end.
	void mediaStoppedByUser();
    void show_logo_signal(bool b);
	void showMessage(QString text);
	void showMessage(QString text, int time);
	void menusNeedInitialize();
	void widgetsNeedUpdate();
	void videoEqualizerNeedsUpdate();
	void audioEqualizerNeedsUpdate();
    void showTime(double sec, bool flag);//kobe

	void positionChanged(int); // To connect a slider

	void newDuration(double); // Duration has changed
	void showFrame(int frame);
	void ABMarkersChanged(int secs_a, int secs_b);
    void bitrateChanged(int vbitrate, int abitrate);
	void needResize(int w, int h);
	void noVideo();
	void volumeChanged(int);

    void audioTracksInitialized();

	//! Sent when requested to play, but there is no file to play
	void noFileToPlay();

	//! MPlayer started but finished with exit code != 0
	void mplayerFinishedWithError(int exitCode);

	//! MPlayer didn't started or crashed
	void mplayerFailed(QProcess::ProcessError error);

	// Resend signal from mplayerprocess:
	void failedToParseMplayerVersion(QString line_with_mplayer_version);

	//! A new line from the mplayer output is available
	void logLineAvailable(QString);

	void receivedForbidden();

protected:
	PlayerProcess * proc;
	MplayerWindow * mplayerwindow;

//#ifndef NO_USE_INI_FILES
    FileSettingsBase * file_settings;
//    FileSettings * file_settings;
//	FileSettingsBase * tv_settings;
//#endif

//#if defined(Q_OS_WIN) || defined(Q_OS_OS2)
//#ifdef SCREENSAVER_OFF
//	WinScreenSaver * win_screensaver;
//#endif
//#endif

private:
	// Some variables to proper restart
	bool we_are_restarting;

	bool just_loaded_external_subs;
	bool just_unloaded_external_subs;
	State _state;
	bool change_volume_after_unpause;

	QString initial_subtitle;

//#if DVDNAV_SUPPORT
//	bool dvdnav_title_is_menu;
//#endif

	QMap<QString,QString> forced_titles;
    QString m_snap;
};

#endif
