/**
 *
 *  Main Haiku js functions
 *
 */ 
;(function($){



    /**
     *  Text players
     */
    $.fn.textPlayers = function() {

        var player = $('div.haiku-text-player');

        player.each(function(){

            var $this, id, container, play, pause, stop,
                loopPlayer, mp3File, ogaFile, selectors;

            $this = $(this);
            id = this.id.match(/haiku-text-player(\d+)/)[1];
            container = $this.next('.haiku-container-' + id);

            play  = container.find('#haiku-play-' + id);
            pause = container.find('#haiku-pause-' + id);
            stop  = container.find('#haiku-stop-' + id);

            loopPlayer = false;

            if(container.is('.haiku-loop'))
                loopPlayer = true;

            selectors = {
                play: '#haiku-play-'  + id,
                pause: '#haiku-pause-' + id,
                stop: '#haiku-stop-'  + id,
                duration: '#haiku-duration-' + id,
                currentTime: '#haiku-current-time-' + id,
                noSolution: '.haiku-no-solution'
            };

            mp3File = play.attr('href');

            if( play.attr('data-haikuoga') ) {
                
                ogaFile = play.attr('data-haikuoga');

                $this.jPlayer({
                    ready: function () {
                        $(this).jPlayer('setMedia', {
                            mp3: mp3File,
                            oga: ogaFile
                        })
                    },
                    swfPath: haiku_jplayerswf_path,
                    supplied: 'mp3, oga',
                    solution: 'html, flash',
                    cssSelectorAncestor: '.haiku-container-' + id,
                    cssSelector: selectors,
                    loop: loopPlayer
                }); // this.jplayer

            } else {

                $this.jPlayer({
                    ready: function () {
                        $(this).jPlayer('setMedia', {
                            mp3: mp3File
                        })
                    },
                    swfPath: haiku_jplayerswf_path,
                    supplied: 'mp3',
                    solution: 'html, flash',
                    cssSelectorAncestor: '.haiku-container-' + id,
                    cssSelector: selectors,
                    loop: loopPlayer
                }); // this.jplayer

            } // endif oga

        }); // player.each
    }



    /**
     *  Graphical players
     */
    $.fn.graphPlayers = function() {

        var player = $('div.haiku-graphical-player')

        player.each(function(){

            var $this, id, container, play, pause, stop, 
                mp3File, ogaFile, selectors, loopPlayer;

            $this = $(this);
            id = this.id.match(/haiku-graphical-player-(\d+)/)[1];
            container = $this.next('.haiku-container-' + id);
            parent = $(this).parent(player);

            play  = container.find('#haiku-play-' + id);
            pause = container.find('#haiku-pause-' + id);
            stop  = container.find('#haiku-stop-' + id);

            loopPlayer = false;


            if(container.is('.haiku-loop'))
                loopPlayer = true;

            selectors = {
                play: '#haiku-play-'  + id,
                pause: '#haiku-pause-' + id,
                stop: '#haiku-stop-'  + id,
                seekBar: '#haiku-seek-bar-' + id,
                playBar: '#haiku-play-bar-' + id,
                duration: '#haiku-duration-' + id,
                currentTime: '#haiku-current-time-' + id,
                noSolution: '.haiku-no-solution'
            };

            mp3File = play.attr('href');

            if( play.attr('data-haikuoga') ) {
                
                ogaFile = play.attr('data-haikuoga')

                $this.jPlayer({
                    ready: function () {
                        $(this).jPlayer('setMedia', {
                            mp3: mp3File,
                            oga: ogaFile
                        })
                    },
                    swfPath: haiku_jplayerswf_path,
                    supplied: 'mp3, oga',
                    solution: 'html, flash',
                    cssSelectorAncestor: '.haiku-container-' + id,
                    cssSelector: selectors,
                    loop: loopPlayer
                }); // this.jplayer

            } else {

                $this.jPlayer({
                    ready: function () {
                        $(this).jPlayer('setMedia', {
                            mp3: mp3File
                        })
                    },
                    swfPath: haiku_jplayerswf_path,
                    supplied: 'mp3',
                    solution: 'html, flash',
                    cssSelectorAncestor: '.haiku-container-' + id,
                    cssSelector: selectors,
                    loop: loopPlayer
                }); // this.jplayer

            } // endif oga


            // lil layout fix for some browsers
            pause.hide().css('padding-right','0');

            // play button
            play.click(function(){

                $(this).hide();
                pause.fadeIn(100);
                parent.jPlayer('play');

                return false;
            });

            // pause button
            pause.click(function(){

                $(this).hide();
                play.fadeIn(100).attr('title','Resume');
                parent.jPlayer('pause');

                return false;
            });

            // stop button
            stop.click(function(){

                pause.hide();
                play.fadeIn(100);
                parent.jPlayer('stop');

                return false;
            });

        }); // player.each

    }

    $(window).load(function(){

        $.fn.textPlayers();
        $.fn.graphPlayers();

    })

})(jQuery);  