/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 14:47:55 by marvin            #+#    #+#             */
/*   Updated: 2025/08/09 17:00:02 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**tokens_to_argv(t_token *start)
{
	t_token	*cur;
	char	**argv;
	int		count;
	int		i;

	cur = start;
	count = 0;
	i = 0;
	while (cur && (cur->type == COMMAND || cur->type == WORD))
	{
		count++;
		cur = cur->next;
	}
	argv = malloc(sizeof(char *) * (count + 1));
	if (!argv)
		return (NULL);
	cur = start;
	while (i < count)
	{
		argv[i++] = ft_strdup(cur->value);
		cur = cur->next;
	}
	argv[i] = NULL;
	return (argv);
}

void	free_argv(char **argv)
{
	int		i;
	
	i = 0;
	while (argv && argv[i])
		free(argv[i++]);
	free(argv);
}

int	is_builtin(char *command)
{
	if (command && (ft_strcmp(command, "echo") == 0
		|| ft_strcmp(command, "cd") == 0
		|| ft_strcmp(command, "pwd") == 0
		|| ft_strcmp(command, "export") == 0
		|| ft_strcmp(command, "unset") == 0
		|| ft_strcmp(command, "env") == 0
		|| ft_strcmp(command, "exit") == 0))
		return (1);
	return (0);
}

void	run_builtin(char **argv, t_data *data)
{
	if (!argv || !argv[0])
		return ;
	if (ft_strcmp(argv[0], "echo") == 0)
		handle_echo_command(argv);
	else if (ft_strcmp(argv[0], "cd") == 0)
		handle_cd_command(argv, data);
	else if (ft_strcmp(argv[0], "pwd") == 0)
		handle_pwd_command();
	else if (ft_strcmp(argv[0], "export") == 0)
		handle_export_command(argv, data);
		else if (ft_strcmp(argv[0], "unset") == 0)
		handle_unset_command(argv, data);
	else if (ft_strcmp(argv[0], "env") == 0)
		handle_env_command(argv, data);
	else if (ft_strcmp(argv[0], "exit") == 0)
		handle_exit_command(argv, data);
}

void	execute_command(t_token *command, t_data *data)
{
	char	**argv;
	
	argv = tokens_to_argv(command);
	if (!argv || !argv[0])
	{
		free_argv(argv);
		return ;
	}
	if (is_builtin(argv[0]))
		run_builtin(argv, data);
	// else
	// 	run_external(argv, data);
	free_argv(argv);
}
