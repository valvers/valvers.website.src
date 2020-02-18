function fillContributors(evt) {
    $( '.contributors img[data-src]' ).each( function() {
        src = $(this).attr("data-src");
        $(this).attr('src',src);
    });
    $('.metadata').detach().insertAfter( '#content h1:first' );
}

if(window.attachEvent) {
    window.attachEvent('onload', fillContributors);
} else {
    if(window.onload) {
        var curronload = window.onload;
        var newonload = function(evt) {
            curronload(evt);
            fillContributors(evt);
        };
        window.onload = newonload;
    } else {
        window.onload = fillContributors;
    }
}
