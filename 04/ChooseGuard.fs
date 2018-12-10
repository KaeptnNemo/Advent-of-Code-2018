open Parser

type Guard = {
    TotalSleepTime : int
    SleepByMinute  : int array
}
with
    static member create() = {
        TotalSleepTime = 0
        SleepByMinute  = Array.create 60 0
    }

type GuardTracker = {
    mutable LastId : int
    guards : Map<int,Guard>
}

[<EntryPoint>]
let main args =
    let events = "input.txt" |> parseInputs
    0