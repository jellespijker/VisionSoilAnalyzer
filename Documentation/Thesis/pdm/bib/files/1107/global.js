(function($) {
	"use strict";

	$(document).ready(function() {

		/* superFish */
		$("ul.sf-menu").superfish({
			autoArrows: false,
			dropShadows: false
		});
		
		/* Mobile menu */
		$('.mobile-menu-toggle').sidr({
			name: 'sidr-main',
			source: '#sidr-close, #navigation',
			side: 'left',
			displace: false
		});
		
		$(".sidr-class-toggle-sidr-close").click( function() {
			$.sidr('close', 'sidr-main');
			preventDefaultEvents: false
		});
		
		/*lightbox*/
		$("a.fancybox").fancybox({
			openEffect: 'elastic',
			closeEffect: 'elastic',
			padding : 10,
			margin : 40,
			helpers : {
				title : {
					type : 'inside'
				},
				overlay : {
					css : {
						'background' : 'rgba(0, 0, 0, 0.75)'
					}
				}
			}
		});
		
		/* scroll to top */
		$(window).scroll(function () {
			if ($(this).scrollTop() > 100) {
				$('a[href=#toplink]').fadeIn();
			} else {
				$('a[href=#toplink]').fadeOut();
			}
		});
		$('a[href=#toplink]').on('click', function(){
			$('html, body').animate({scrollTop:0}, 'normal');
			return false;
		});
		
		/* animate comments scroll */
		$(".comment-scroll a").click(function(event){
			event.preventDefault();
			$('html,body').animate({scrollTop:$(this.hash).offset().top}, 'normal');
		});

		$('#single-media-wrap').imagesLoaded(function(){
			$('.flexslider-gallery').flexslider({
				animation: "fade",
				controlNav: false,
				slideshow: true,
				smoothHeight: true,
				slideDirection: "horizontal",
				prevText: "",
				nextText: "",
				start: function(slider) {
					$('.flexslider-gallery li img').click(function(event){
						event.preventDefault();
						slider.flexAnimate(slider.getTarget("next"));
					});
				}
			});
		});

	});

	$(window).load(function() {

		$('div#load-more').addClass('display-element');
		
		$('#wpex-grid-wrap, .single .post-video').animate({opacity:1},'fast');
		
		$('.grid-loader').hide();
		
		var $container = $('#wpex-grid-wrap');
		$container.imagesLoaded(function(){
			$container.isotope({
				itemSelector: '.loop-entry',
				transformsEnabled: false,
				animationOptions: {
					duration: 400,
					easing: 'swing',
					queue: false
				}
			});
		});
		
		$(window).resize(function () {
			var $container = $('#wpex-grid-wrap');
			$container.isotope();
		});
		
		var ajaxurl = wpexvars.ajaxurl;
		$('div#load-more').click(function() {
			$(this).children('a').html(wpexvars.loading);
			var $this = $(this),
				anchor = $this.children('a'),
				nonce = anchor.val(),
				pagenum = anchor.data('pagenum'),
				maxpage = anchor.data('maxpage'),
				data = {
					action: 'aq_ajax_scroll',
					pagenum: pagenum,
					archive_type: anchor.data('archive_type'),
					archive_id: anchor.data('archive_id'),
					archive_month: anchor.data('archive_month'),
					archive_year: anchor.data('archive_year'),
					post_format: anchor.data('post_format'),
					author: anchor.data('author'),
					s: anchor.data('s'),
					security: nonce
				};
			$.post(ajaxurl, data, function(response) {
				var content = $(response);
				$(content).imagesLoaded(function() {
					$('div#load-more a').html(wpexvars.loadmore);
					$('#wpex-grid-wrap').append(content).isotope( 'appended', content, function() {	
						$('#wpex-grid-wrap').isotope('reLayout');
						$(".fitvids").fitVids(); /* re-fire fitvids */
					});
				});
				anchor.data('pagenum', pagenum + 1);
				if(pagenum >= maxpage) {
					$this.fadeOut();
				}
			});
			return false;
		});

		function wpex_staticheader() {
			var $header_height = $('.fixed-header').outerHeight();
			$('.fixed-header').css({
				position: 'fixed',
				top: 0,
				left: 0
			});
			$('#wrap').css({
				paddingTop: $header_height
			});	
		}
		
		wpex_staticheader();
		
		$(window).resize(function () {
			wpex_staticheader();
		});
		
		$(window).bind('orientationchange', function(event) {
			var $header_height = $('.fixed-header').outerHeight();
			$('#wrap').css({
				paddingTop: $header_height
			});
		});

		if ($(window).width() > 767) {
			$('.wpex-tooltip').tipsy({ fade: true, gravity: 's' });
		}
		
		
	});

})(jQuery);
