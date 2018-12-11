open Parser

type Guard = {
    TotalSleepTime : int
    SleepByMinute  : int array
}
with
    static member create () = {
        TotalSleepTime = 0
        SleepByMinute  = Array.create 60 0
    }

    static member sleptForMinute m self =
        self.SleepByMinute.[m] <- self.SleepByMinute.[m] + 1
        {self with TotalSleepTime = self.TotalSleepTime + 1 }

    static member sleptFromTo f t self =
        seq { f .. t }
        |> Seq.fold (fun acc m -> Guard.sleptForMinute m acc) self

type GuardTracker = {
    LastId : int
    FellAsleepMin : int
    Guards : Map<int,Guard>
}
with
    static member create () = {
        LastId = -1
        FellAsleepMin = 0
        Guards = Map.empty
    }

    static member fallAsleep h m self =
        { self with FellAsleepMin = if h <> 0 then 0 else m }

    static member wakeUp h m self =
        if h <> 0
            then self
        else
            { self with
                Guards =
                    Map.add
                        self.LastId
                        (Guard.sleptFromTo self.FellAsleepMin (m-1)
                            (Map.find self.LastId self.Guards))
                        self.Guards 
        }

    static member beginShift i self =
        let contains = self.Guards |> Map.tryFind i |> Option.isSome
        { self with
            LastId = i
            FellAsleepMin = 0
            Guards = if contains then self.Guards else Map.add i (Guard.create()) self.Guards
        }

    static member consumeEvent self e =
        self
        |> match e.Type with
            | FallsAsleep -> GuardTracker.fallAsleep e.Date.Hour e.Date.Minute
            | WakesUp -> GuardTracker.wakeUp e.Date.Hour e.Date.Minute
            | ShiftBegin i -> GuardTracker.beginShift i

[<EntryPoint>]
let main args =
    let tracker =
        "input.txt"
        |> parseInputs
        |> Array.fold GuardTracker.consumeEvent (GuardTracker.create())

    let guards = seq {
        for KeyValue(k,v) in tracker.Guards do
            yield (k,v)
    }

//  part 1
//    let maxGuard = Seq.maxBy (fun (i,g) -> g.TotalSleepTime) guards

//  part 2
    let maxGuard = Seq.maxBy (fun (i,g) -> Array.max g.SleepByMinute) guards

    let maxMinute = 
        (snd maxGuard).SleepByMinute
        |> Seq.ofArray
        |> Seq.mapi (fun i x -> (i,x))
        |> Seq.maxBy snd
        |> fst

    printfn "%i" ((fst maxGuard) * maxMinute)
    0