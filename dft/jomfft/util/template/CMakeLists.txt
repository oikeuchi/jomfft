cmake_minimum_required(VERSION 3.21)

target_sources(${OBJLIB} PRIVATE
CopyDef.h
CopyImpl.h
KeyedListDef.h
KeyedListImpl.h
ListDef.h
ListImpl.h
UndefCopyMacro.h
UndefKeyedListMacro.h
UndefListMacro.h
)