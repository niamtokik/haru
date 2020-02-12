-module(haru).
% -export([start/0, start/1]).
% -export([start_link/0, start_link/1]).
% -export([new/0, new/1]).
-export([init/1]).
-export([handle_call/3, handle_cast/2, handle_info/2]).
-behavior(gen_server).

init(_Args) ->
    process_flag(trap_exit, true),
    Port = open_port({spawn, haru}, [{packer, 2}]),
    {ok, Port}.

handle_call(Message, From, State) ->
    io:format("~p~n", [Message]),
    {reply, Message, State}.

handle_cast(Message, State) ->
    io:format("~p~n", [Message]),
    {noreply, State}.

handle_info(Message, State) ->
    io:format("~p~n", [Message]),
    {noreply, State}.





