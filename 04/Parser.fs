module Parser

open System.Text.RegularExpressions
open System.IO

let intArrFromRegex pattern text =
    let reg = Regex pattern
    let m   = reg.Match text
    m.Groups |> Seq.tail |> Seq.map (fun g -> g.ToString() |> int) |> Seq.toArray

type Date = { Year: int; Month: int; Day: int; Hour: int; Minute: int}
with
    static member create s =
        let groups = intArrFromRegex "\[([0-9]+)-([0-9]+)-([0-9]+) ([0-9]+):([0-9]+)\]" s
        {
            Year   = groups.[0]
            Month  = groups.[1]
            Day    = groups.[2]
            Hour   = groups.[3]
            Minute = groups.[4]
        }

    static member toTimeStamp self =
        sprintf "%i%i%i%i%i" self.Year self.Month self.Day self.Hour self.Minute

type EventType =
    | ShiftBegin of int
    | FallsAsleep
    | WakesUp
with
    static member create s =
        let getGuardId () =
            let groups = intArrFromRegex "Guard #([0-9]+) begins shift" s
            groups.[0]
        match s with
        | "falls asleep" -> FallsAsleep
        | "wakes up"     -> WakesUp
        | _ -> ShiftBegin (getGuardId())

type Event = { Type: EventType; Date: Date }
with
    static member create s =
        let reg = Regex "(\[.+\]) (.+)"
        let m   = reg.Match s
        let parts = m.Groups |> Seq.tail |> Seq.map (fun g -> g.ToString()) |> Seq.toArray
        {
            Type = EventType.create parts.[1]
            Date = Date.create parts.[0]
        }

let parseInputs =
    File.ReadLines
    >> Seq.map Event.create
    >> Seq.toArray
    >> Array.sortBy (fun e -> Date.toTimeStamp e.Date)