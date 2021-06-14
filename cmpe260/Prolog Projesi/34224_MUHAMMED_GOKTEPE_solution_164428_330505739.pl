% muhammed goktepe
% 2017400162
% compiling: yes
% complete: yes

% artist(ArtistName, Genres, AlbumIds).
% album(AlbumId, AlbumName, ArtistNames, TrackIds).
% track(TrackId, TrackName, ArtistNames, AlbumName, [Explicit, Danceability, Energy,
%                                                    Key, Loudness, Mode, Speechiness,
%                                                    Acousticness, Instrumentalness, Liveness,
%                                                    Valence, Tempo, DurationMs, TimeSignature]).


% getArtistTracks(+ArtistName, -TrackIds, -TrackNames) 5 points

% albumFeatures(+AlbumId, -AlbumFeatures) 5 points
% artistFeatures(+ArtistName, -ArtistFeatures) 5 points

% trackDistance(+TrackId1, +TrackId2, -Score) 5 points
% albumDistance(+AlbumId1, +AlbumId2, -Score) 5 points
% artistDistance(+ArtistName1, +ArtistName2, -Score) 5 points

% findMostSimilarTracks(+TrackId, -SimilarIds, -SimilarNames) 10 points
% findMostSimilarAlbums(+AlbumId, -SimilarIds, -SimilarNames) 10 points
% findMostSimilarArtists(+ArtistName, -SimilarArtists) 10 points

% filterExplicitTracks(+TrackList, -FilteredTracks) 5 points

% getTrackGenre(+TrackId, -Genres) 5 points

% discoverPlaylist(+LikedGenres, +DislikedGenres, +Features, +FileName, -Playlist) 30 points

features([explicit-0, danceability-1, energy-1,
          key-0, loudness-0, mode-1, speechiness-1,
       	  acousticness-1, instrumentalness-1,
          liveness-1, valence-1, tempo-0, duration_ms-0,
          time_signature-0]).

filter_features(Features, Filtered) :- features(X), filter_features_rec(Features, X, Filtered).
filter_features_rec([], [], []).
filter_features_rec([FeatHead|FeatTail], [Head|Tail], FilteredFeatures) :-
    filter_features_rec(FeatTail, Tail, FilteredTail),
    _-Use = Head,
    (
        (Use is 1, FilteredFeatures = [FeatHead|FilteredTail]);
        (Use is 0,
            FilteredFeatures = FilteredTail
        )
    ).

%appends an element to a list
%append2(+Ff,+Sim,-[Ff|Sim]).
append2(Ff,Sim,[Ff|Sim]).

%adiing track names recursively.
addTrackNames([],Sub,TrackNames,EternalLast) :- EternalLast = Sub.
addTrackNames([A|B],Sub,TrackNames,EternalLast) :- track(A,NAME,_,_,_), append2(NAME,Sub,TrackNames), addTrackNames(B,TrackNames,L,EternalLast). 
%taking track ıds from albums and add them to a list
%getTrack(+AlbumIDS,-C,-AllTracks,-LastAllTracks)
getTrack([],C,AllTracks,LastAllTracks) :- LastAllTracks = C.
getTrack([H0|T0],C,AllTracks,LastAllTracks) :- album(H0,_,_,S), append(S,C,AllTracks), getTrack(T0,AllTracks,_,LastAllTracks).


getArtistTracks(ArtistName,TrackIds,TrackNames) :- findall(IDS,artist(ArtistName,_,IDS),[AlbumIDS|Tail]), C = [], getTrack(AlbumIDS,C,AllTracks,LastAllTracks), reverse(LastAllTracks,TrackIds),
    Sub = [], addTrackNames(TrackIds,Sub,LastNames,EternalLast), reverse(EternalLast,TrackNames).

%arithDiv(+TotalFeatures,+Len,-AlbumFeatures) it makes arithmetic division
arithDiv([K|[L|[M|[N|[O|[P|[R|[S|Tail2]]]]]]]],Len,AlbumFeatures) :- Nf1 is K/Len, Nf2 is L/Len, Nf3 is M/Len, Nf4 is N/Len, Nf5 is O/Len, Nf6 is P/Len, Nf7 is R/Len, Nf8 is S/Len,
    AlbumFeatures = [Nf1,Nf2,Nf3,Nf4,Nf5,Nf6,Nf7,Nf8].

%getFeatures(+TrackIds,-TotalFeatures,0,0,0,0,0,0,0,0) it adds all matching futeres and calculetes total feature
getFeatures([],Total,A,B,C,D,E,F,G,H) :- Total = [A,B,C,D,E,F,G,H].

getFeatures([Head|Tail],Total,Ftr1,Ftr2,Ftr3,Ftr4,Ftr5,Ftr6,Ftr7,Ftr8) :- track(Head,_,_,_,Features), filter_features(Features,FilteredOne), [FilHead|FilTail] = FilteredOne, Sum1 is Ftr1 + FilHead,
    [H1|T1] = FilTail, Sum2 is Ftr2 + H1, [H2|T2] = T1, Sum3 is Ftr3 + H2, [H3|T3] = T2, Sum4 is Ftr4 + H3, [H4|T4] = T3, Sum5 is Ftr5 + H4, [H5|T5] = T4, Sum6 is Ftr6 + H5, 
    [H6|T6] = T5, Sum7 is Ftr7 + H6, [H7|T7] = T6, Sum8 is Ftr8 + H7, getFeatures(Tail,Total,Sum1,Sum2,Sum3,Sum4,Sum5,Sum6,Sum7,Sum8).


albumFeatures(AlbumId, AlbumFeatures) :- findall(IDS,album(AlbumId,_,_,IDS),[TrackIds|T]), getFeatures(TrackIds,TotalFeatures,0,0,0,0,0,0,0,0), 
    length(TrackIds,Len), arithDiv(TotalFeatures,Len,AlbumFeatures).

artistFeatures(ArtistName, ArtistFeatures) :- getArtistTracks(ArtistName,Xx,Yy), getFeatures(Xx,TotalFeatures,0,0,0,0,0,0,0,0), length(Xx,Len), 
    arithDiv(TotalFeatures,Len,ArtistFeatures).

trackDistance(TrackId1, TrackId2, Score) :- track(TrackId1,_,_,_,Features1), track(TrackId2,_,_,_,Features2), filter_features(Features1,Filtered1), filter_features(Features2,Filtered2),
    [K|[L|[M|[N|[O|[P|[R|[S|Tail2]]]]]]]] = Filtered1, [A|[B|[C|[D|[E|[F|[G|[H|Tail1]]]]]]]] = Filtered2, D1 is (K - A)*(K - A),D2 is (L - B)*(L - B),D3 is (M - C)*(M - C),D4 is (N - D)*(N - D),
    D5 is (O - E)*(O - E),D6 is (P - F)*(P - F),D7 is (R - G)*(R - G),D8 is (S - H)*(S - H), Summ is D1+D2+D3+D4+D5+D6+D7+D8 , Score is sqrt(Summ).

albumDistance(AlbumId1, AlbumId2, Score) :- albumFeatures(AlbumId1, AlbumFeatures1), albumFeatures(AlbumId2, AlbumFeatures2), [K|[L|[M|[N|[O|[P|[R|[S|Tail2]]]]]]]] = AlbumFeatures1, 
    [A|[B|[C|[D|[E|[F|[G|[H|Tail1]]]]]]]] = AlbumFeatures2, D1 is (K - A)*(K - A),D2 is (L - B)*(L - B),D3 is (M - C)*(M - C),D4 is (N - D)*(N - D),
    D5 is (O - E)*(O - E),D6 is (P - F)*(P - F),D7 is (R - G)*(R - G),D8 is (S - H)*(S - H), Summ is D1+D2+D3+D4+D5+D6+D7+D8 , Score is sqrt(Summ).

artistDistance(ArtistName1, ArtistName2, Score) :- artistFeatures(ArtistName1, ArtistFeatures1), artistFeatures(ArtistName2, ArtistFeatures2), [K|[L|[M|[N|[O|[P|[R|[S|Tail2]]]]]]]] = ArtistFeatures1, 
    [A|[B|[C|[D|[E|[F|[G|[H|Tail1]]]]]]]] = ArtistFeatures2, D1 is (K - A)*(K - A),D2 is (L - B)*(L - B),D3 is (M - C)*(M - C),D4 is (N - D)*(N - D),
    D5 is (O - E)*(O - E),D6 is (P - F)*(P - F),D7 is (R - G)*(R - G),D8 is (S - H)*(S - H), Summ is D1+D2+D3+D4+D5+D6+D7+D8 , Score is sqrt(Summ).

%similarTracks(+TrackId,+Second,-NewSim,-X) calculate distances and append them to a list 
similarTracks(TrackId, [], SimTracks,X) :- X = SimTracks.
similarTracks(TrackId, [First|Second], SimTracks,X) :- trackDistance(TrackId,First,Score), append2(Score-First,SimTracks,NewSim), similarTracks(TrackId,Second,NewSim,X).
%lastPhase(+NewList,1,+D,+E,-SimilarIds,-SimilarNames) reverses the two list then takes first 30 tracks that are similar to eachother
lastPhase(_,32,SimIds,SimNms,SimilarIds,SimilarNames) :- reverse(SimIds,NewSimIds), reverse(SimNms,NewSimNms), [E|F] = NewSimIds, [G|H] = NewSimNms, SimilarIds = F, SimilarNames = H.
lastPhase([B|K],A,SimIds,SimNms,I,N) :- B = Key-Value, append2(Value,SimIds,X), track(Value,Name,_,_,_), append2(Name,SimNms,Y), A1 is A+1, lastPhase(K,A1,X,Y,I,N).

findMostSimilarTracks(TrackId, SimilarIds, SimilarNames) :- findall(A,track(A,_,_,_,_),ALLTRACKS), SimTracks = [], similarTracks(TrackId, ALLTRACKS, SimTracks,X), keysort(X,NewList), 
    lastPhase(NewList,1,D,E,SimilarIds,SimilarNames).

% similarAlbums(+AlbumId,+Second,-NewSim,-X) calculate distances and append them to a list 
similarAlbums(AlbumId, [], SimAlbums,X) :- X = SimAlbums.
similarAlbums(AlbumId, [First|Second], SimAlbums,X) :- albumDistance(AlbumId,First,Score), append2(Score-First,SimAlbums,NewSim), similarAlbums(AlbumId,Second,NewSim,X).
%lastPhaseAlbum(+NewList,1,+D,+E,-SimilarIds,-SimilarNames).  reverses the two list then takes first 30 albums that are similar to eachother
lastPhaseAlbum(_,32,SimIds,SimNms,SimilarIds,SimilarNames) :- reverse(SimIds,NewSimIds), reverse(SimNms,NewSimNms), [E|F] = NewSimIds, [G|H] = NewSimNms, SimilarIds = F, SimilarNames = H.
lastPhaseAlbum([B|K],A,SimIds,SimNms,I,N) :- B = Key-Value, append2(Value,SimIds,X), album(Value,Name,_,_), append2(Name,SimNms,Y), A1 is A+1, lastPhaseAlbum(K,A1,X,Y,I,N).

findMostSimilarAlbums(AlbumId, SimilarIds, SimilarNames) :- findall(A,album(A,_,_,_),ALLALBUMS), SimAlbums = [], similarAlbums(AlbumId, ALLALBUMS, SimAlbums,X), keysort(X,NewList), 
     lastPhaseAlbum(NewList,1,D,E,SimilarIds,SimilarNames).

%similarArtist(+ArtistName,+Second,-NewSim,-X) calculate distances and append them to a list 
similarArtist(ArtistName, [], SimArtist,X) :- X = SimArtist.
similarArtist(ArtistName, [First|Second], SimArtist,X) :- artistDistance(ArtistName,First,Score), append2(Score-First,SimArtist,NewSim), similarArtist(ArtistName,Second,NewSim,X).
%lastPhaseArtist(+NewList,1,+D,-SimilarArtists).  reverses the list then takes first 30 artist that are similar to eachother
lastPhaseArtist(_,32,SimNms,SimilarArtists) :- reverse(SimNms,NewSimNms), [G|H] = NewSimNms, SimilarArtists = H.
lastPhaseArtist([B|K],A,SimNms,I) :- B = Key-Value, append2(Value,SimNms,Y), A1 is A+1, lastPhaseArtist(K,A1,Y,I).

findMostSimilarArtists(ArtistName, SimilarArtists) :- findall(A,artist(A,_,_),ALLARTIST), SimArtist = [], similarArtist(ArtistName, ALLARTIST, SimArtist,X), keysort(X,NewList),
    lastPhaseArtist(NewList,1,D,SimilarArtists).


filterChecker([],FilteredTracks) :- FilteredTracks = [].
filterChecker([H|T],FilteredTracks) :- track(H,_,_,_,[EXP|A]), filterChecker(T,FilteredTracks1), (((EXP == 0) -> (FilteredTracks = [H|FilteredTracks1])); FilteredTracks = FilteredTracks1).
% sonunda variable adresi kalıyor onu düzeltmeden başlama diğerlerine.

filterExplicitTracks(TrackList, FilteredTracks) :- filterChecker(TrackList,FilteredTracks).


getGenre([],GenreSet) :- GenreSet = [].
getGenre([H|T],GenreSet) :- artist(H,Genre,_), getGenre(T,Genree), union(Genree,Genre,NewList),GenreSet = NewList.

getTrackGenre(TrackId, Genres) :- track(TrackId,_,Artists,_,_), getGenre(Artists,GenreSet), Genres = GenreSet.

%searchh(+Head,+LikedGenres,-Y) searches liked and disliked genres in list and if it presentt in the list returns any number other than zero
searchh(A,[Head1|Tail1],C) :- searchh(A,Tail1,X), ((sub_atom(A,_,_,_,Head1) -> C is X+1); (C is X)).
searchh(A,[],C) :- C is 0.
%findDislikedInTrack(+Tail,+DislikedGenres,-X) find disliked genres in tracks and changes X to any number other than zero
findDislikedInTrack([Bas|Kuy],DislikedGenres,C) :- findDislikedInTrack(Kuy,DislikedGenres,X), searchh(Bas,DislikedGenres,Y), C is X+Y.
findDislikedInTrack([],DislikedGenres,C) :- C is 0.

%findLikedInTrack(+Tail,+LikedGenres,-X) find liked genres in tracks and changes X to any number other than zero
findLikedInTrack([Head|Tail],LikedGenres,C) :- findLikedInTrack(Tail,LikedGenres,X), searchh(Head,LikedGenres,Y), C is X+Y.
findLikedInTrack([],LikedGenres,C) :- C is 0.


getLikedTracks([],LikedGenres,DislikedGenres,LikedList) :- LikedList =[].
getLikedTracks([H|T],LikedGenres,DislikedGenres,LikedList) :- getTrackGenre(H,Genres), findLikedInTrack(Genres,LikedGenres,IsLiked), findDislikedInTrack(Genres,DislikedGenres,IsDisliked), 
        getLikedTracks(T,LikedGenres,DislikedGenres,LikedListee), (((IsLiked >0 , IsDisliked == 0) -> LikedList =[H|LikedListee]); (LikedList = LikedListee)).

%calculatess(+FilteredOne,+Features,-Score) makes the math of calculation
calculatess(FilteredOne,Features,Score) :- [K|[L|[M|[N|[O|[P|[R|[S|Tail2]]]]]]]] = FilteredOne, [A|[B|[C|[D|[E|[F|[G|[H|Tail1]]]]]]]] = Features, D1 is (K - A)*(K - A),D2 is (L - B)*(L - B),D3 is (M - C)*(M - C),
    D4 is (N - D)*(N - D), D5 is (O - E)*(O - E),D6 is (P - F)*(P - F),D7 is (R - G)*(R - G),D8 is (S - H)*(S - H), Summ is D1+D2+D3+D4+D5+D6+D7+D8 , Score is sqrt(Summ).

%calculatess(+FilteredOne,+Features,-Score) calculates distances between given feature and tracks feature.
calculateDistance([],Features,DistanceList) :- DistanceList = [].
calculateDistance([Head|Tail],Features,DistanceList) :- track(Head,_,_,_,TrackFeature), filter_features(TrackFeature,FilteredOne), calculatess(FilteredOne,Features,Score), calculateDistance(Tail,Features,DistanceListee),
    A = Score-Head, DistanceList = [A|DistanceListee].
%getFirst30(+K,+A1,-TrackId1,-TrackName1,-Artists1,-Distance1) add First 30 elements to the list
getFirst30(_,1,TrackId,TrackName,Artists,Distance) :- TrackId = [],TrackName = [],Artists = [], Distance = [].
getFirst30([B|K],A,TrackId,TrackName,Artists,Distance) :- B = Key-Value, track(Value,Name,Owner,_,_), A1 is A-1, getFirst30(K,A1,TrackId1,TrackName1,Artists1,Distance1), TrackId = [Value|TrackId1], 
        TrackName =[Name|TrackName1], Artists = [Owner|Artists1], Distance = [Key|Distance1].

discoverPlaylist(LikedGenres, DislikedGenres, Features, FileName, Playlist) :- findall(A,track(A,_,_,_,_),Tracks), getLikedTracks(Tracks,LikedGenres,DislikedGenres,LikedList),
        calculateDistance(LikedList,Features,DistanceList), keysort(DistanceList, NewDistanceList), getFirst30(NewDistanceList,31,Playlist,TrackName,Artists,Distance), open(FileName, write, Stream), writeln(Stream, Playlist), 
        writeln(Stream,TrackName), writeln(Stream,Artists), writeln(Stream,Distance), close(Stream).