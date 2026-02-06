import React, { useEffect, useState } from 'react';
import { io, Socket } from 'socket.io-client';
import { AreaChart, Area, ResponsiveContainer, YAxis, Tooltip } from 'recharts';
import { Activity, Anchor, Github } from 'lucide-react';

interface Process {
  pid: number;
  name: string;
  cpu: number;
}

interface MonitorData {
  cpu: number;
  ram: number;
  ram_used: number;
  ram_total: number;
  processes: Process[];
}

interface ChartEntry extends MonitorData {
  time: string;
}

const socket: Socket = io('http://localhost:4000');

function App() {
  const [stats, setStats] = useState<ChartEntry[]>([]);

  useEffect(() => {
    socket.on('stats', (data: MonitorData) => {
      setStats((prev) => {
        const newEntry: ChartEntry = {
          ...data,
          time: new Date().toLocaleTimeString().slice(3, 8)
        };
        return [...prev, newEntry].slice(-40);
      });
    });

    return () => {
      socket.off('stats');
    };
  }, []);

  const latest: ChartEntry = stats.length > 0 
    ? stats[stats.length - 1] 
    : { cpu: 0, ram: 0, ram_used: 0, ram_total: 0, processes: [], time: '--:--' };

  const sortedProcesses = latest.processes 
    ? [...latest.processes]
        .map(proc => ({
          ...proc,
          simulatedLoad: parseFloat(((proc.pid % 5) + (latest.cpu / 20)).toFixed(1))
        }))
        .sort((a, b) => b.simulatedLoad - a.simulatedLoad)
    : [];

  return (
    <div className="min-h-screen bg-black text-slate-400 p-10 font-mono">
      
      <div className="max-w-5xl mx-auto mb-10 flex items-center justify-between border-b border-slate-800 pb-6">
        <div>
          <h1 className="text-2xl font-bold tracking-tighter uppercase flex items-center gap-3 text-white">
            <Activity className="animate-pulse text-blue-500" /> Linux System Core
          </h1>
          <p className="text-slate-500 text-sm mt-1 uppercase tracking-widest">
            Real-time performance monitoring via C++ & Node.js
          </p>
          <a 
            href="https://github.com/1Filipek2/linux-system-monitor" 
            target="_blank" 
            rel="noopener noreferrer"
            className="flex items-center gap-2 text-[9px] text-slate-600 hover:text-blue-400 transition-colors group mt-2"
          >
            <Github size={12} className="group-hover:rotate-12 transition-transform" />
            <span className="tracking-widest uppercase italic">VIEW SOURCE CODE & README</span>
          </a>
        </div>
        <div className="text-right">
          <span className="border border-green-500 px-2 py-1 text-green-500 font-bold uppercase text-[10px] tracking-tighter">
            System Status: OK
          </span>
        </div>
      </div>

      <div className="max-w-5xl mx-auto">
        
        <div className="grid grid-cols-2 gap-8 mb-10">
          <div className="text-left border-l-2 border-green-500 bg-green-500/5 p-4">
            <div className="text-[10px] text-green-700 uppercase mb-1 font-bold">CPU_LOAD</div>
            <div className="text-3xl font-black text-green-400 tracking-tighter">
              {latest.cpu.toFixed(1)}%
            </div>
          </div>
          <div className="text-left border-l-2 border-amber-500 bg-amber-500/5 p-4">
            <div className="flex justify-between items-start">
              <div className="text-[10px] text-amber-700 uppercase mb-1 font-bold">RAM_USAGE</div>
              <div className="text-[10px] text-amber-900 font-bold">
                {(latest.ram_used || 0).toFixed(1)}GB / {(latest.ram_total || 0).toFixed(1)}GB
              </div>
            </div>
            <div className="text-3xl font-black text-amber-400 tracking-tighter">
              {latest.ram.toFixed(1)}%
            </div>
          </div>
        </div>
        
        <div className="h-32 w-full mb-12">
          <ResponsiveContainer width="100%" height="100%">
            <AreaChart data={stats}>
              <defs>
                <linearGradient id="colorCpu" x1="0" y1="0" x2="0" y2="1">
                  <stop offset="5%" stopColor="#22c55e" stopOpacity={0.3}/>
                  <stop offset="95%" stopColor="#22c55e" stopOpacity={0}/>
                </linearGradient>
                <linearGradient id="colorRam" x1="0" y1="0" x2="0" y2="1">
                  <stop offset="5%" stopColor="#f59e0b" stopOpacity={0.2}/>
                  <stop offset="95%" stopColor="#f59e0b" stopOpacity={0}/>
                </linearGradient>
              </defs>
              <YAxis domain={[0, 100]} hide />
              <Tooltip 
                contentStyle={{ backgroundColor: '#000', border: '1px solid #222' }} 
                itemStyle={{ fontSize: '10px', textTransform: 'uppercase' }}
              />
              <Area 
                type="monotone" 
                dataKey="ram" 
                stroke="#f59e0b" 
                fillOpacity={1} 
                fill="url(#colorRam)" 
                strokeWidth={1} 
                strokeDasharray="3 3" 
                isAnimationActive={false} 
              />
              <Area 
                type="monotone" 
                dataKey="cpu" 
                stroke="#22c55e" 
                fillOpacity={1} 
                fill="url(#colorCpu)" 
                strokeWidth={2} 
                isAnimationActive={false} 
              />
            </AreaChart>
          </ResponsiveContainer>
        </div>

        <div className="relative w-full h-112.5 bg-linear-to-b from-[#0a192f] to-black border-2 border-blue-900/30 rounded-xl overflow-hidden shadow-2xl">
          {sortedProcesses.map((proc, index) => {
            const fishTypes = ['🐠','🐬', '🐟', '🦈', '🐡'];
            const fishEmoji = fishTypes[index % fishTypes.length];
            const isEven = index % 2 === 0;
            const randomY = 15 + (index * 14); 
            const fixedDuration = 12 + (index * 2); 

            return (
              <div 
                key={proc.pid || index}
                className="absolute flex flex-col items-center group/fish cursor-crosshair transition-all duration-1000"
                style={{ 
                  left: isEven ? '5%' : '80%',
                  top: `${randomY}%`,
                  animation: `${isEven ? 'swim' : 'swim-reverse'} ${fixedDuration}s infinite alternate ease-in-out`,
                  animationDelay: `${index * 0.7}s`,
                }}
              >
                <div className="opacity-0 group-hover/fish:opacity-100 transition-opacity duration-150">
                  <div className="w-px h-32 bg-white/10 absolute bottom-full left-1/2"></div>
                  <Anchor size={16} className="text-white/60 -mb-2 animate-bounce" />
                </div>

                <span className={`text-5xl select-none drop-shadow-[0_5px_15px_rgba(0,0,0,0.5)] ${isEven ? 'scale-x-[-1]' : ''}`}>
                  {fishEmoji}
                </span>
                
                <div className="mt-2 text-[10px] tracking-widest flex gap-3 items-center bg-black/60 backdrop-blur-sm px-2 py-1 border border-white/10 rounded-sm text-white">
                  <span className="font-bold">{proc.name}</span>
                  <span className="text-green-500 font-black">{proc.simulatedLoad}%</span>
                </div>
              </div>
            );
          })}
        </div>

        <div className="mt-6 text-[9px] text-slate-700 uppercase tracking-[0.4em] flex justify-between">
          <span>{">"} Kernel_Source_Connected (findo-dev.me)</span>
          <span>{">"} TS_TypeSafety_Active</span>
        </div>
      </div>
    </div>
  );
}

export default App;